import os, json

props_path = '.vscode/c_cpp_properties.json'
props_exists = os.path.exists(props_path)

print('C/CPP properties exists:', props_exists, '("%s")' % props_path)

if not props_exists:
    quit()

info_path = 'build/conanbuildinfo.txt'
info_exists = os.path.exists(info_path)

print('Conan build info exists:', info_exists, '("%s")' % info_path)

if not info_exists:
    quit()

info = [line.strip() for line in open(info_path).readlines()]

def grab_section(list, name):
    location = list.index('[%s]' % name)
    short = list[location+1:]
    short = short[:short.index('')]
    print(name, '->', ', '.join(short))
    return short

includes = grab_section(info, 'includedirs')
defines = grab_section(info, 'defines')

doc = json.loads(open(props_path).read())

if not 'configurations' in doc or len(doc['configurations']) == 0:
    print('No configurations found in C/CPP properties file.')
    quit()

doc_includes = [entry for entry in doc['configurations'][0]['includePath'] if entry.startswith('$')]

for include in includes:
    doc_includes.append(include)

doc['configurations'][0]['includePath'] = doc_includes

open(props_path, 'w').write(json.dumps(doc, indent=4))