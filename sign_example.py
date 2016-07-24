import hmac, hashlib, time

def sign(key, msg):
    return hmac.new(key, msg.encode('utf-8'), hashlib.sha256).digest()

key = "secretKey"
url = "/api/v1/page"
t = str(int(time.time()))
key = sign(key, t)
signature = sign(key, url)
