#curl -X POST -F "name=LinosGT" -F "type=testtype" -F "status=locked" "http://localhost:8000/api/cdb_rest/gt"

#curl -X POST http://localhost:8000/api/cdb_rest/gt -H 'Content-Type: application/json' -d '{"name": "LinosGT", "type":"testtype","status":"locked"}'

#curl -X POST -d "name=LinosGT&type=testtype&status=locked" http://localhost:8000/api/cdb_rest/gt
#curl -X POST -d 'status=locked' http://localhost:8000/api/cdb_rest/gt

#curl 'http://nginx-ruslan.apps.rcf.bnl.gov/api/cdb_rest/payloadiovs/?gtName=PromptGT&majorIOV=100&minorIOV=97'
#curl http://localhost:8000/api/cdb_rest/payloadiovs/?gtName=my_gt&majorIOV=100&minorIOV=97

curl -X POST http://localhost:8000/api/cdb_rest/gt -H 'Content-Type: application/json' -d '{"name": "LinosGT", "status":"unlocked"}'
curl http://localhost:8000/api/cdb_rest/globalTag/LinosGT
curl -X POST http://localhost:8000/api/cdb_rest/pt -H 'Content-Type: application/json' -d '{"name": "LinosType"}'
curl -X POST http://localhost:8000/api/cdb_rest/pl -H 'Content-Type: application/json' -d '{"payload_type": "LinosType"}'
curl -X PUT http://localhost:8000/api/cdb_rest/pl_attach -H 'Content-Type: application/json' -d '{"payload_list": "LinosType_7", "global_tag": "LinosGT"}'

curl -X POST http://localhost:8000/api/cdb_rest/piov -H 'Content-Type: application/json' -d '{"payload_url": "/path/file.txt", "major_iov":0, "minor_iov":0}'
curl -X POST http://localhost:8000/api/cdb_rest/piov -H 'Content-Type: application/json' -d '{"payload_url": "/path/ohterfile.txt", "major_iov":0, "minor_iov":0}'
curl -X PUT http://localhost:8000/api/cdb_rest/piov_attach -H 'Content-Type: application/json' -d '{"payload_list": "LinosType_7", "piov_id":76}'


curl -X POST http://localhost:8000/api/cdb_rest/gtstatus -H 'Content-Type: application/json' -d '{"name": "locked"}'
curl -X PUT http://localhost:8000/api/cdb_rest/gt_change_status/LinosGT/locked