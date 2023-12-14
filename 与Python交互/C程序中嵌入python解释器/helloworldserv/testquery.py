import requests as rq

with open("testscript.py", "r") as f:
    code = f.read()
rs = rq.post("http://localhost:18080/submit",json={
    "code": code
})
print(rs.status_code)
if rs.status_code == 200:
    print(rs.json())