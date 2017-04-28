import smtplib
import json
import gnupg
import sys
from email.message import Message
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart


class EmailSender(object):
    def __init__(self):
        with open('cred.json') as json_data:
            self.data = json.load(json_data)
        self.SERVER = self.data['server']
        self.FROM = self.data['from']
        self.server = smtplib.SMTP(self.SERVER)

    def open_conn(self):
        self.server.ehlo()
        self.server.starttls()
        self.server.login(self.data['from'], self.data['password'])

    def messageFromSignature(self, signature):
        message = Message()
        message['Content-Type'] = 'application/pgp-signature; name="signature.asc"'
        message['Content-Description'] = 'OpenPGP digital signature'
        message.set_payload(signature)
        return message

    def send_msg(self, TO, dom, IP):
        SUBJECT = "DNS server issue"
        TEXT = """Hi,\n\nMy name is {myName} and I am a Telecommunications student at Warsaw Uniwersity of Technology. \
I am doing a research on DNS servers and I noticed that your site is vulnerable to unauthorized AXFR zone transfer. \
It means that anyone at the Internet can see your DNS server's records. You can google what that could cause. Below I\
send you the data that could identify your vulnerable server:\n
domain name(s): {domain}\n
nameserver IP(s): {nsIp}\n
I would be grateful if you for any feedback from you.\n\n\
---\n{myName}\n{organization}\n{university}""".format(myName=self.data['myName'], organization=self.data['organization'],
                                                      university=self.data['uni'], nsIp=", ".join(IP),
                                                      domain=", ".join(dom))

        message = """%s
        """ % (TEXT)

        gpg_passphrase = sys.argv[1]

        basemsg = MIMEText(message)

        gpg = gnupg.GPG(homedir='/home/marcin/.gnupg')
        basetext = basemsg.as_string().replace('\n', '\r\n')
        signature = str(gpg.sign(basetext, detach=True, passphrase=gpg_passphrase))
        if signature:
            signmsg = self.messageFromSignature(signature)
            msg = MIMEMultipart(_subtype="signed", micalg="pgp-sha1",
                                protocol="application/pgp-signature")
            msg.attach(basemsg)
            msg.attach(signmsg)
            msg['Subject'] = "Subject"
            msg['From'] = self.FROM
            msg['To'] = TO[0]
            #print(msg.as_string(unixfrom=True))  # or send
            self.server.sendmail(self.FROM, TO, msg.as_string(unixfrom=True))
        else:
            print('Warning: failed to sign the message!')

        
    def end_conn(self):
        self.server.quit()

es = EmailSender()
es.open_conn()
es.send_msg(["marcin.skw@gmail.com"], ["testdomain"], ["0.0.0.0"])
es.end_conn()

