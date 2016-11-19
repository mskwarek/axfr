"""
WSGI config for sweetspot.
"""

import os
import sys
os.environ.setdefault("DJANGO_SETTINGS_MODULE", "settings")

sys.path.append('/home/mkoi/mgr/myDig/python_module/')
sys.path.append('/home/mkoi/mgr/myDig/python_module/web')
sys.path.append('/home/mkoi/mgr/myDig/python_module/client')


from django.core.wsgi import get_wsgi_application
application = get_wsgi_application()
