import requests
import json


base_url = "http://127.0.0.1:8000/api/cdb_rest"
url = base_url + '/pl_attach'

pl_attach = {
    'global_tag': 'linos_gt',
    'payload_list': 'linos_pl_type_22'
}

print(json.dumps(pl_attach))

#r = requests.put(url=url, json=pl_attach)
#print(r.json())

#curl -X PUT http://localhost:8000/api/cdb_rest/pl_attach -d "{'global_tag':'linos_gt', 'payload_list':'linos_pl_type_22'}"
