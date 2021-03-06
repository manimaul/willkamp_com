#REST API (public - no auth required)

####GET /{fileName}.(png|jpg|html|js)
* response *file mimetype* the file

####GET /api/v1/public/record
**Retrieve a record by id**
* required param *query* col - the database collection
* required param *query* oid - the record's object id
* response *json* - the record response  
`{"record": {}}`

####POST /api/v1/public/find_record
**Retrieve a record by query**
* required param *query* col - the database collection
* required parm *body* find - the query
* response *json* - the record response  
`{"record": {}}`

####POST /api/v1/public/find_multi_records
**Retrieve paginated record(s) by query**
* required param *query* col - the database collection
* required param *query* size - page size (number of records) (max 100)
* optional param *query* start - the i-th (index) to start (default 0)
* required parm *body* find - the query
* response *json* - the record list response  
`{"records": [],
  "more": true|false}`
  
#REST API (private - auth required)

All "private" requests require a request signature signed with a secret key.

####POST /api/v1/private/file
**Post a file**
* required param *query* hmac - the hmac
* required param *query* epoch - time in seconds
* required param *form data* file - the file to post

####POST /api/v1/private/record
* required param *query* hmac - the hmac
* required param *query* epoch - time in seconds

####POST /api/v1/private/record_update
* required param *query* hmac - the hmac
* required param *query* epoch - time in seconds

####DELETE /api/v1/private/record
* required param *query* hmac - the hmac
* required param *query* epoch - time in seconds

#REQUEST SIGNING
* sha256 hmac of the key, time and url
key - a secret key
time - the current unix time in seconds
url - the request url e.g. "/api/v1/private/page"

Example in python
```python
import hmac, hashlib, time

def sign(key, msg):
    return hmac.new(key, msg.encode('utf-8'), hashlib.sha256).digest()

key = "secretKey"
url = "/api/v1/page"
t = str(int(time.time()))
key = sign(key, t)
signature = sign(key, url)
```