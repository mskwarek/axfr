import smtplib
import json


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

    def send_msg(self, TO, dom, IP):
        SUBJECT = "DNS server issue"
        TEXT = """Hi,\n\nMy name is {myName} and I am a Telecommunications student at Warsaw Uniwersity of Technology. \
I am doing a research on DNS servers and I noticed that your site is vulnerable to unauthorized AXFR zone transfer. \
It means that anyone at the Internet can see your DNS server's records. You can google what that could cause. Below I\
send you the data that could identify your vulnerable server:\n
domain name: {domain}\n
nameserver IP: {nsIp}\n
I would be grateful if you for any feedback from you.\n\n\
---\n{myName}\n{organization}\n{university}""".format(myName=self.data['myName'], organization=self.data['organization'],
                                                      university=self.data['uni'], nsIp=IP, domain=dom)

        message = """From: %s\r\nTo: %s\r\nSubject: %s\r\n\

        %s
        """ % (self.FROM, ", ".join(TO), SUBJECT, TEXT)
        self.server.sendmail(self.FROM, TO, message)

    def end_conn(self):
        self.server.quit()

es = EmailSender()
es.open_conn()
es.send_msg([])
es.end_conn()
