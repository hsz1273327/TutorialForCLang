import requests as rq

rs = rq.post("http://localhost:18080/submit", files=[
    ('script', ('testscript.py', open('testscript.py', 'rb'), 'application/python'))])
print(rs.status_code)
if rs.status_code == 200:
    print(rs.json())