curl -X POST http://localhost:8000/api/cdb_rest/gtstatus -d '{"name":"mStatus"}' -H "Content-Type: application/json"
curl -X POST http://localhost:8000/api/cdb_rest/gttype -d '{"name":"mType"}' -H "Content-Type: application/json"
curl -X POST http://localhost:8000/api/cdb_rest/gt -d '{"name":"mGT", "status": "mStatus", "type": "mType"}' -H "Content-Type: application/json"


curl -X POST http://linostest.apps.usatlas.bnl.gov/api/cdb_rest/gtstatus -d '{"name":"mStatus"}' -H "Content-Type: application/json"
curl -X POST http://linostest.apps.usatlas.bnl.gov/api/cdb_rest/gt -d '{"name":"mGT", "status": "mStatus", "type": "mType"}' -H "Content-Type: application/json"


curl -X POST http://linostest.apps.usatlas.bnl.gov/api/cdb_rest/gttype -d '{"name":"mType"}' -H "Content-Type: application/json"

