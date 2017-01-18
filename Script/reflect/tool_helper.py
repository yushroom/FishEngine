from __future__ import print_function
import re
import os

'''CamelCase -> CamelCase
m_fieldOfView -> Field Of View
'''
def CamelCaseToReadable(string):
	# http://stackoverflow.com/questions/1175208/elegant-python-function-to-convert-camelcase-to-snake-case
	if string.startswith('m_'):
		string = string[2:]
	if string[0].islower():
		string = string[0].upper() + string[1:]
	return re.sub('((?<=[a-z0-9])[A-Z]|(?!^)[A-Z](?=[a-z]))', r' \1', string)

def unittest_CamelCaseToReadable():
	for s in ("m_fieldOfView", "CamelCase", "Camel2Camel2Case", "getHTTPResponseCode", "get2HTTPResponse123Code", "HTTPResponseCodeXYZ"):
		print(s,'==>', CamelCaseToReadable(s))


def UpdateFile(out_path, content):
	need_update = True
	if os.path.exists(out_path):
		with open(out_path) as f:
			old_content = f.read()
			need_update = content!= old_content
	if need_update:
		print("update", out_path)
		with open(out_path, 'w') as f:
			f.write(content)
	else:
		print("no update", out_path)

if __name__ == "__main__":
	unittest_CamelCaseToReadable()