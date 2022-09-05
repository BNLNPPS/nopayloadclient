#curl -X POST -F "name=LinosGT" -F "type=testtype" -F "status=locked" "http://localhost:8000/api/cdb_rest/gt"

curl -X POST http://localhost:8000/api/cdb_rest/gt -H 'Content-Type: application/json' -d '{"name": "LinosGT", "type":"testtype","status":"locked"}'

#curl -X POST -d "name=LinosGT&type=testtype&status=locked" http://localhost:8000/api/cdb_rest/gt
curl -X POST -d 'status=locked' http://localhost:8000/api/cdb_rest/gt
