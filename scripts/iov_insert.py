import requests

# Configurable
base_url = "http://127.0.0.1:8000/api/cdb_rest"
IGNORE_MAJOR_IOV = 1

def iov_insert(gt_name, calibration_type, calibration_file, iov_start, iov_end=-1):

    # Start the workflow, sanity checks first, check the GT exists
    url = base_url + '/globalTags'
    r = requests.get(url = url)
    all_global_tags = r.json()
    all_GT_names = []
    for GT in all_global_tags:
        all_GT_names.append(GT['name'])

    if gt_name not in all_GT_names:
        print("Global tag ", gt_name, " not found, exiting")
        print(all_global_tags)
        print(all_GT_names)
        exit(1)

    print("Global tag ", gt_name, " found in ", all_GT_names)

    # Now check the payload type is valid
    url = base_url + "/gtPayloadLists/" + gt_name     # GT Detail API, return status, type and PayloadList objects, no PIOVs
    r = requests.get(url = url)
    PL_NAMES = r.json()
    if calibration_type not in PL_NAMES: # eh, types are keys so this works
        print("Payload type ", calibration_type, " not found, exiting")
        print(PL_NAMES)
        exit(1)

    print("Payload type ", calibration_type, " found in ", PL_NAMES)

    # Looks like a valid request, check the file exists and upload to storage

    url_filename = check_and_upload_file(gt_name, calibration_type, calibration_file, iov_start)
        
    # Create payload_iov object
    print("Create PayloadIOVs")
    url = base_url + '/piov'
    piov_id = -1
    piov = {}
    # Should really master the tertiary operator in python !
    if iov_end != -1:
        piov = {
            'payload_url': url_filename,
            'major_iov': IGNORE_MAJOR_IOV,
            'minor_iov': iov_start,
            'major_iov_end': IGNORE_MAJOR_IOV,
            'minor_iov_end': iov_end
        }
    else:
        piov = {
            'payload_url': url_filename,
            'major_iov': IGNORE_MAJOR_IOV,
            'minor_iov': iov_start
        }
        

    # If successful you get the ID back
    r = requests.post(url = url, json=piov)
    if r.status_code == 200:
        piov_id = r.json()['id']
    else:
        print("Could not create the PIOV object, need to exit")
        print(r.status_code)
        print(r.json())
        exit(1)

    print(r.json())

    # Attach payload_iov object to its payload_list (determined by payload type)
    print("Attach Payload_IOV object to its Payload list")
    url = base_url + '/piov_attach'
    piov_attach = {
        'payload_list' : PL_NAMES[calibration_type],
        'piov_id'      : piov_id
    }
    r = requests.put(url = url, json=piov_attach)

    if r.status_code == 200:
        print(r.json())
    else:
        print("Could not attach the PIOV object, need to exit")
        print(r.status_code)
        print(r.json())
        exit(1)
    
    return


    
def check_and_upload_file(gt_name, calibration_type, calibration_file, iov_start):

    print("Checking local file exists ", calibration_file)

    # Algo to create a filename on storage
    storage_filename = create_storage_filename(gt_name, calibration_type, calibration_file, iov_start)
    print("Created storage filename = ", storage_filename)    
    # execute the copy, if success
    print("Uploaded file to storage")

    return storage_filename
    

def create_storage_filename(gt_name, calibration_type, calibration_file, iov_start):
    return calibration_file
