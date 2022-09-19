import requests
  
# Payload Types are user-defined, examples from Chris
PL_TYPEs = ['Beam', 'FieldMap', 'CEMC_Thresh', 'ZDC', 'CEMC_Geo']

# Payloads for those types, examples from Chris
PL_PAYLOADS = {
    'Beam'        : "D0DXMagnets.dat",
    'FieldMap'    : "sphenix3dbigmapxyz.root",
    'CEMC_Thresh' : "CEMCprof_Thresh30MeV.root",
    'ZDC'         : "towerMap_ZDC.txt",
    'CEMC_Geo'    : "cemc_geoparams-0-0-4294967295-1536789215.xml"
}

GT_NAME = "sPHENIX_ExampleGT_1"

base_url = "http://127.0.0.1:8000/api/cdb_rest"

# Create a global tag type object - this should be a VERY rare occurence
print("Create Global Tag Type")
gttype = {'name':'testtype'} # id should be autoincr, name should be unique
url = base_url + '/gttype'
r = requests.post(url = url, json=gttype)
print(r.json())


# Create a global tag status object - this should be a VERY rare occurence
print("Create Global Tag Status")
gtstatus = {'name':'locked'} # id should be autoincr, name should be unique
url = base_url + '/gtstatus'
r = requests.post(url = url, json=gtstatus)
print(r.json())
gtstatus = {'name':'unlocked'} # id should be autoincr, name should be unique
url = base_url + '/gtstatus'
r = requests.post(url = url, json=gtstatus)
print(r.json())


# Create a global tag object - this should be QUITE rare
print("Create Global Tag Object")
gt = {
    'name': GT_NAME,
    'status': "unlocked",
    'type': "testtype"
}
url = base_url + '/gt'
r = requests.post(url = url, json=gt)
print(r.json())


# Create payload_type objects - these should also be VERY rare
print("Create Payload Types")
url = base_url + '/pt'
for pl_type in PL_TYPEs:
    pt = {'name':pl_type} # id should be autoincr, name should be unique
    r = requests.post(url = url, json=pt)
    print(r.json())

# Create payload_list objects (based only on payload_types) - this is QUITE rare
print("Create Payload Lists")
pll_names = {}
url = base_url + '/pl'
for pl_type in PL_TYPEs:
    pllist = {
        'payload_type': pl_type,
    } # Propose 'name' should be payload_type + autoincr, 'global_tag' default is not set yet
    r = requests.post(url = url, json=pllist)
    datar = r.json()
    pll_names[pl_type] = datar['name']
    print(r.json())

# Attach a payload_list to a global tag object, this is QUITE rare
print("Attach Payload lists to Global Tag")
url = base_url + '/pl_attach'
for pl_type, pll_name in pll_names.items(): #pl_type not used in attach json, but checked in method
    pl_attach = {
        'global_tag' : GT_NAME,
        'payload_list' : pll_name
    }
    # IF Global tag is locked then this should fail
    # check GT_NAME and pll_name exist
    # get payload_type of payload_list and if pl_type exists then this is a replace (update) operation
    # change 'global_tag' field in the payload_list object to GT_NAME (and to null for the replaced list)
    r = requests.put(url = url, json=pl_attach)
    print(r.json())


# Now I have associated all of my payload_lists to the global tag, lock it - this should be QUITE rare
print("Locking Global Tag")
url = base_url + '/gt_change_status/'
url = url + GT_NAME
url = url + "/locked"
r = requests.put(url = url)
print(r.json())

#pll_names = {'ZDC': u'ZDC_309', 'Beam': u'Beam_306', 'CEMC_Geo': u'CEMC_Geo_310', 'CEMC_Thresh': u'CEMC_Thresh_308', 'FieldMap': u'FieldMap_307'}

print(f'pll_names = {pll_names}')

# Create payload_iov objects, typically this is the operation that will happen over and over again using automated tools so this needs to be efficient
print("Create PayloadIOVs")
url = base_url + '/piov'
piov_ids = {}
for pl_type, pll_name in pll_names.items():
    piov = {
#        'payload_url': "blabla",
        'payload_url': PL_PAYLOADS[pl_type],
        'major_iov':  1,
        'minor_iov': 1,
#        'major_iov_end':  16,
#        'minor_iov_end': 16,
    }#  'payload_list' attribute is set later via a new attach method, same as for payload_lists-> global_tags
    # Need to return the id
    print(f'url = {url}')
    print(f'json = {piov}')
    r = requests.post(url = url, json=piov)
    print(f'r = {r}')
    print(r.json())
    piov_ids[pll_name] = r.json()['id']

# Attach payload_iov objects to a payload_list, also happens a lot and needs to be quite efficient
print("Attach Payload_IOV objects to their Payload lists")
url = base_url + '/piov_attach'
for pll_name, piov_id in piov_ids.items():
    piov_attach = {
        'payload_list' : pll_name,
        'piov_id'      : piov_id
    } # check pll_name and piov_id, check locked status for logic and decide if insert ok or not
    r = requests.put(url = url, json=piov_attach)
    print(r.json())
