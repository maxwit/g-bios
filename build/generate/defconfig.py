#!/usr/bin/env python3
#
# The Main Program for Building Embedded Development Environment
#
# http://www.maxwit.com
# http://maxwit.github.com
# http://maxwit.googlecode.com
#

import os, sys, re, random, socket, fcntl, struct
from xml.etree import ElementTree

config = {}

def traverse(node):
	if node.tag == 'choice':
		lst = node.getchildren()

		if node.attrib['name'] in config:
			default_node = config[node.attrib['name']]
		else:
			if 'default' in node.attrib:
				default_node = node.attrib['default']
			else:
				default_node = lst[0].attrib['name']
			config[node.attrib['name']] = default_node

		for n in lst:
			if n.attrib['name'] == default_node:
				traverse(n)
			# FIXME: check matched or not
	elif node.tag == 'select':
		l2 = node.text.split(' ') # FIXME: re.split
		for s in l2:
			config[s] = 'y'
	else:
		dfs = True
		if node.tag == 'config':
			try: # if 'name' in node.attrib: # if -> try:
				key = node.attrib['name']
				if key in config:
					if 'string' in node.attrib:
						config[key] = '"' + node.attrib['string'] + '"'
					elif 'bool' in node.attrib:
						if node.attrib['bool'] == "y":
							config[key] = node.attrib['bool']
						else:
							dfs = False
					elif 'int' in node.attrib:
						config[key] = node.attrib['int']
					else:
						print('Invalid node!', node)
			except:
				print('Invalid node!', node)

		if dfs == True:
			lst = node.getchildren()
			for n in lst:
				traverse(n)

def get_ip_address(ifname):
	s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

	try:
		local_ip = fcntl.ioctl(s.fileno(), 0x8915, # SIOCGIFADDR
					struct.pack('256s', ifname[:15]))
	except IOError:
		return "10.0.0.1"

	return socket.inet_ntoa(local_ip[20:24])

def parse_config(fn):
	try:
		tree = ElementTree.parse(fn)
	except:
		print("fail to parse " + fn + "!")
		exit(1)

	root = tree.getroot()
	lst = root.getchildren()
	for n in lst:
		traverse(n)

def get_attr(substr, fd):
	for line in fd:
		if re.match(substr, line) != None:
			elem = re.split('\s*=\s*', line.replace('\n',''))
			return elem[1]
	return None

#FIXME
def get_active_nic():
	return "eth0"

#FIXME
def get_net_mask(nic):
	return "255.255.255.0"

def generate_netmask(ip):
	return "255.255.255.0"

if __name__ == "__main__":
	if os.getenv('USER') == 'root':
		print('cannot run as root!')
		exit(1)

	if len(sys.argv) != 2:
		print("Usage: ...")
		exit(1)

	fn_def_cfg = 'build/configs/arm/' + sys.argv[1]

	try:
		fd_def_cfg = open(fn_def_cfg)
	except:
		print('fail to open "%s"' % fn_def_cfg)
		# parse_config("build/configs/configs.xml")
		exit(1)

	for line in fd_def_cfg:
		if re.match(r'^CONFIG_', line) != None:
			elem = re.split('\s*=\s*', re.sub('^CONFIG_', '', line.replace('\n','')))
			config[elem[0]] = elem[1]

	fd_def_cfg.close()

	parse_config("build/configs/configs.xml")

	# print(config)
	# print('configure %s (%s).' % (config['BOARD'], config['ARCH']))

	try:
		fd_dot_cfg = open('.config', 'w')
	except:
		print('fail to open .config file')
		exit(1)

	for x in config:
		fd_dot_cfg.write('CONFIG_' + x + ' = ' + config[x] + '\n')

	fd_dot_cfg.close()
