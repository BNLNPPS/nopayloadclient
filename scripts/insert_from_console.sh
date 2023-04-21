curl -X POST http://localhost:8000/api/cdb_rest/gtstatus -d '{"name":"unlocked"}' -H "Content-Type: application/json"
curl -X POST http://localhost:8000/api/cdb_rest/gt -d '{"name":"mGT", "status": "unlocked"}' -H "Content-Type: application/json"
curl -X POST http://localhost:8000/api/cdb_rest/gt -d '{"name":"mGT", "status": "unlocked"}' -H "Content-Type: application/json"

curl -X POST http://localhost:8000/api/cdb_rest/piov -d '{"payload_url": "/tmp/file.dat", "major_iov": 42, "minor_iov": 69, "checksum": "cs"}' -H "Content-Type: application/json"
