import json
import sys

import ast
from docker import Client
import logging
import traceback
from uuid import getnode as get_mac

class GestionDocker:
    cli = Client(base_url='unix://var/run/docker.sock')

    def SetSocket(self, socket):
        self.Socket = socket
        
    def StringToBool(self, v):
        return str(v).lower() in ("yes", "true", "t", "1")

    def showcontainer(self, param):
        _quiet = None
        _all = None
        _trunc = None
        _latest = None
        _since = None
        _before = None
        _limit = None
        _size = None
        _filter = None
        try:
            option = param['option']
            for i in option:
                if(i.get('quiet') != None):
                    _quiet = self.StringToBool(i.get('quiet'))
                if(i.get('all') != None):
                    _all = self.StringToBool(i.get('all'))
                if(i.get('trunc') != None):
                    _trunc = self.StringToBool(i.get('trunc'))
                if(i.get('latest') != None):
                    _latest = self.StringToBool(i.get('latest'))
                if(i.get('since') != None):
                    _since = str(i.get('since'))
                if(i.get('before') != None):
                    _before = str(i.get('before'))
                if(i.get('limit') != None):
                    _limit = int(i.get('limit'))
                if(i.get('size') != None):
                    _size = bool(i.get('size'))
                if(i.get('filter') != None):
                    _filter = (i.get('filter'))
        except KeyError:
            print 'no opt'
        ExecCode = ''
        try:
            ExecCode = self.cli.containers(quiet=_quiet, all=_all, trunc=_trunc, latest=_latest, since=_since, before=_before, limit=_limit, size=_size, filters=_filter)
        except Exception as e:
            ExecCode = json.dumps({'showcontainer': str(e)})
            ExecCode = json.dumps({'ERROR' : ExecCode})
            return ExecCode
        ExecCode = json.dumps({'showcontainer': ExecCode})
        print 'Container : ' + str(ExecCode)
        return ExecCode

    def Showmac(self, param):
        ExecCode = self.Socket.get_mac_addr()
        ExecCode = json.dumps({'mac': str(ExecCode)})
        return ExecCode

    def Showimage(self, param):
        _name = None
        _quiet = None
        _all = None
        _filters = None
        try:
            option = param['option']
            for i in option:
                if(i.get('name') != None):
                    _name = str(i.get('name'))
                if(i.get('quiet') != None):
                    _quiet = self.StringToBool(i.get('quiet'))
                if(i.get('all') != None):
                    _all = self.StringToBool(i.get('all'))
                if(i.get('filters') != None):
                    _filters = ast.literal_eval(i.get('filters'))
        except KeyError:
            print 'no opt'
        ExecCode =''
        try:
            ExecCode = self.cli.images(name=_name, quiet=_quiet, all=_all, filters=_filters)
        except Exception as e:
            ExecCode = json.dumps({'showimage': str(e)})
            ExecCode = json.dumps({'ERROR' : ExecCode})
            return ExecCode
        ExecCode = json.dumps({'showimage': ExecCode})
        print "Image : " + str(ExecCode)
        return ExecCode

    def Deleteimage(self, param):
        _image = ''
        _force = False
        _noprune = False
        try:
            option = param['option']
            for i in option:
                if(i.get('image') != None):
                    _image = str(i.get('image'))
                if(i.get('force') != None):
                    _force = self.StringToBool(i.get('force'))
                if(i.get('noprune') != None):
                    _noprune = self.StringToBool(i.get('noprune'))
        except KeyError:
            print 'no opt'
        try:
            ExecCode = self.cli.remove_image(image=_image, force=_force, noprune=_noprune)
        except Exception as e:
            ExecCode = json.dumps({'deleteimage': str(e)})
            ExecCode = json.dumps({'ERROR' : str(ExecCode)})
            return ExecCode
        ExecCode = json.dumps({'deleteimage': ExecCode})
        print 'Delete Img : ' + str(ExecCode)
        return ExecCode
    
    def StopContainer(self, param):
        _container = ''
        _timeout = 0
        try:
            option = param['option']
            for i in option:
                if(i.get('container') != None):
                    _container = str(i.get('container'))
                if(i.get('timeout') != None):
                    _timeout = self.StringToBool(i.get('timeout'))
        except KeyError:
            print 'no opt'
        ExecCode = 'Ok'
        try:
            ExecCode = self.cli.stop(container=_container, timeout=_timeout)
        except Exception as e:
            ExecCode = json.dumps({'stopcontainer': str(e)})
            ExecCode = json.dumps({'ERROR' : str(ExecCode)})
            return ExecCode
        ExecCode = json.dumps({'stopcontainer': ExecCode})
        print 'StopContainer : ' + str(ExecCode)
        return ExecCode
    
    def CreateHostConfig(self, param):
        _bind = None
        _port_bindings = None
        _lxc_conf = None
        _oom_kill_disable = None
        _publish_all_ports = None
        _links = None
        _privilege = None
        _dns = None
        _dns_search = None
        _volumes_from = None
        _network_mode = None
        _restart_policy = None
        _cap_add = None
        _cap_drop = None
        _extra_hosts = None
        _read_only = None
        _pid_mode = None
        _ipc_mode = None
        _security_opt = None
        _ulimits = None
        _log_config = None
        _mem_limit = None
        _memswap_limit = None
        _mem_swappiness = None
        _cpu_group = None
        _cpu_period = None
        _group_add = None
        _devices = None

        try:
            option = param['option']
            for i in option:
                if(i.get('bind') != None):
                    _bind = str(i.get('bind'))
                if(i.get('port_bindings') != None):
                    _port_bindings = ast.literal_eval(i.get('port_bindings'))
                if(i.get('lxc_conf') != None):
                    _lxc_conf = ast.literal_eval(i.get('lxc_conf'))
                if(i.get('oom_kill_disable') != None):
                    _oom_kill_disable = self.StringToBool(i.get('oom_kill_disable'))
                if(i.get('publish_all_ports') != None):
                    _publish_all_ports = self.StringToBool(i.get('publish_all_ports'))
                if(i.get('links') != None):
                    _links = ast.literal_eval(i.get('links'))
                if(i.get('privilege') != None):
                    _privilege = self.StringToBool(i.get('privilege'))
                if(i.get('dns') != None):
                    _dns = str(i.get('dns')).split()
                if(i.get('dns_search') != None):
                    _dns_search = str(i.get('dns_search')).split()
                if(i.get('volumes_from') != None):
                    _volumes_from = str(i.get('volumes_from'))
                if(i.get('network_mode') != None):
                    _network_mode = str(i.get('network_mode'))
                if(i.get('restart_policy') != None):
                    _restart_policy = ast.literal_eval(i.get('restart_policy'))
                if(i.get('cap_add') != None):
                    _cap_add = str(i.get('cap_add'))
                if(i.get('cap_drop') != None):
                    _cap_drop = str(i.get('cap_drop'))
                if(i.get('extra_hosts') != None):
                    _extra_hosts = ast.literal_eval(i.get('extra_hosts'))
                if(i.get('read_only') != None):
                    _read_only = self.StringToBool(i.get('read_only'))
                if(i.get('pid_mode') != None):
                    _pid_mode = str(i.get('pid_mode'))
                if(i.get('ipc_mode') != None):
                    _ipc_mode = str(i.get('ipc_mode'))
                if(i.get('security_opt') != None):
                    _security_opt = str(i.get('security_opt'))
                if(i.get('ulimits') != None):
                    _ulimits = ast.literal_eval(i.get('ulimits'))
                if(i.get('log_config') != None):
                    _log_config = ast.literal_eval(i.get('log_config'))
                if(i.get('mem_limit') != None):
                    _mem_limit = int(i.get('mem_limit'))
                if(i.get('memswap_limit') != None):
                    _memswap_limit = int(i.get('memswap_limit'))
                if(i.get('mem_swappiness') != None):
                    _mem_swappiness = int(i.get('mem_swappiness'))
                if(i.get('cpu_group') != None):
                    _cpu_group = int(i.get('cpu_group'))
                if(i.get('cpu_period') != None):
                    _cpu_period = int(i.get('cpu_period'))
                if(i.get('group_add') != None):
                    _group_add = str(i.get('group_add'))
                if(i.get('devices') != None):
                    _devices = str(i.get('devices'))
        except KeyError:
            print 'no opt'
        ExecCode = 'Ok'
        try:
            ExecCode = self.cli.create_host_config(binds=_bind, port_bindings=_port_bindings, lxc_conf=_lxc_conf, oom_kill_disable=_oom_kill_disable, publish_all_ports=_publish_all_ports, links=_links, privileged=_privilege, dns=_dns, dns_search=_dns_search, volumes_from=_volumes_from, network_mode=_network_mode, restart_policy=_restart_policy, cap_add=_cap_add, cap_drop=_cap_drop, extra_hosts=_extra_hosts, read_only=_read_only, pid_mode=_pid_mode, ipc_mode=_ipc_mode, security_opt=_security_opt, ulimits=_ulimits, log_config=_log_config, mem_limit=_mem_limit, memswap_limit=_memswap_limit, mem_swappiness=_mem_swappiness, cpu_period=_cpu_period, group_add=_group_add, devices=_devices)
        except Exception as e:
            ExecCode = json.dumps({'createhost': str(e)})
            ExecCode = json.dumps({'ERROR' : str(ExecCode)})
            return ExecCode
        print 'CreateHost : ' + str(ExecCode)
        return ExecCode

    def DeleteContainer(self, param):
        _container = ''
        _v = False
        _link = False
        _force = False
        try:
            option = param['option']
            for i in option:
                if(i.get('container') != None):
                    _container = str(i.get('container'))
                if(i.get('v') != None):
                    _v = self.StringToBool(i.get('v'))
                if(i.get('link') != None):
                    _link = self.StringToBool(i.get('link'))
                if(i.get('force') != None):
                    _force = self.StringToBool(i.get('force'))
        except KeyError:
            print 'no opt'
        try:
            ExecCode = self.cli.remove_container(container=_container, v=_v, link=_link, force=_force)
        except Exception as e:
            ExecCode = json.dumps({'deletecontainer': str(e)})
            ExecCode = json.dumps({'ERROR' : str(ExecCode)})
            return ExecCode
        ExecCode = json.dumps({'deletecontainer': ExecCode})
        print 'DeleteContainer : ' + str(ExecCode)
        return ExecCode

    def run(self, param):
        _image = ''
        _comand = None
        _hostname = None
        _user = None
        _detach = False
        _stdin_open = False
        _tty = False
        _mem_limit = None
        _ports = None
        _environment = None
        _dns = None
        _volumes = None
        _volumes_from = None
        _network_disabled = False
        _port_bindings = None
        _name = None
        _entrypoint = None
        _cpu_shares = None
        _working_dir = None
        _domainname = None
        _memswap_limit = None
        _host_config = None
        _mac_adress = None
        _labels = None
        _voume_driver = None
        _stop_signal = None
        try:
            option = param['option']
            for i in option:
                if(i.get('image') != None):
                    _image = str(i.get('image'))
                if(i.get('command') != None):
                    _comand = (i.get('command')).split()
                if(i.get('hostname') != None):
                    _hostname = str(i.get('hostname'))
                if(i.get('user') != None):
                    _user = str(i.get('user'))
                if(i.get('detach') != None):
                    _detach = self.StringToBool(i.get('detach'))
                if(i.get('stdin_open') != None):
                    _stdin_open = self.StringToBool(i.get('stdin_open'))
                if(i.get('tty') != None):
                    _tty = self.StringToBool(i.get('tty'))
                if(i.get('mem_limit') != None):
                    _mem_limit = float(i.get('mem_limit'))
                if(i.get('ports') != None):
                    _ports = (i.get('ports'))
                    print 'port : ' + str(_ports)
                if(i.get('environment') != None):
                    _environment = (i.get('environment')).split()
                if(i.get('dns') != None):
                    _dns = (i.get('dns')).split()
                if(i.get('volumes') != None):
                    _volumes = (i.get('volumes')).split()
                if(i.get('volumes_from') != None):
                    _volumes_from = (i.get('volumes_from')).split()
                if(i.get('network_disabled') != None):
                    _network_disabled = self.StringToBool(i.get('network_disabled'))
                if(i.get('name') != None):
                    _name = str(i.get('name'))
                if(i.get('entrypoint') != None):
                    _entrypoint = (i.get('entrypoint')).split()
                if(i.get('cpu_shares') != None):
                    _cpu_shares = int(i.get('cpu_shares'))
                if(i.get('working_dir') != None):
                    _working_dir = str(i.get('working_dir'))
                if(i.get('domainname') != None):
                    _domainname = (i.get('domainname')).split()
                if(i.get('memswap_limit') != None):
                    _memswap_limit = int(i.get('memswap_limit'))
                if(i.get('cpuset') != None):
                    _cpuset = str(i.get('cpuset'))
                if(i.get('port_bindings') != None):
                    _port_bindings = ast.literal_eval(str(i.get('port_bindings')))
                if(i.get('mac_address') != None):
                    _mac_address = str(i.get('mac_address'))
                if(i.get('labels') != None):
                    _labels = str(i.get('labels'))
                if(i.get('volume_driver') != None):
                    _volume_driver = ast.literal_eval(i.get('volume_driver'))
        except KeyError:
            print 'no opt'
        ExecCode = 'Ok'
        try:
            container = self.cli.create_container(image=_image, command=_comand, hostname=_hostname, user=_user, detach=_detach, stdin_open=_stdin_open, tty=_tty, mem_limit=_mem_limit, ports=_ports, environment=_environment, dns=_dns, volumes=_volumes, volumes_from=_volumes_from, network_disabled=_network_disabled, name=_name, entrypoint=_entrypoint, cpu_shares=_cpu_shares, working_dir=_working_dir, domainname=_domainname, memswap_limit=_memswap_limit, cpuset=None, host_config=_host_config, mac_address=_mac_adress, labels=_labels, volume_driver=_voume_driver)
            ExecCode = self.cli.start(container=container.get('Id'), port_bindings=_port_bindings)
        except Exception as e:
            ExecCode = json.dumps({'createcontainer': str(e)})
            ExecCode = json.dumps({'ERROR' : str(ExecCode)})
            return ExecCode
        ExecCode = json.dumps({'createcontainer': ExecCode})
        print 'CreateContainer : ' + str(ExecCode)
        return ExecCode

    def Login(self, param):
        _username = ''
        _password = ''
        _email = ''
        _registry = ''
        _reauth = None
        _insecure_registry = None
        _dockercfg_path = ''

        ExecCode = 'Ok'
        try:
            option = param['option']
            for i in option:
                if(i.get('username') != None):
                    _username = str(i.get('username'))
                if(i.get('password') != None):
                    _password = str(i.get('password'))
                if(i.get('email') != None):
                    _email = str(i.get('email'))
                if(i.get('registry') != None):
                    _registry = str(i.get('registry'))
                if(i.get('reauth') != None):
                    _reauth = self.StringToBool(i.get('reauth'))
                if(i.get('dockercfg_path') != None):
                    _dockercfg_path = str(i.get('dockercfg_path'))

        except KeyError:
            print 'no opt'
        try:
            ExecCode = self.cli.login(username=_username, password=_password, email=_email, registry=_registry, reauth=_reauth, insecure_registry=_insecure_registry, dockercfg_path=_dockercfg_path)
        except Exception as e:
            ExecCode = json.dumps({'login': str(e)})
            ExecCode = json.dumps({'ERROR' : str(ExecCode)})
            return ExecCode
        ExecCode = json.dumps({'login': ExecCode})
        print 'Login : ' + ExecCode
        return ExecCode

    def Pull(self, param):
        _repository = None
        _tag = 'latest'
        _stream = None
        _insecure_registry = None
        _auth_config = None
        try:
            option = param['option']
            for i in option:
                if(i.get('repository') != None):
                    _repository = str(i.get('repository'))
                if(i.get('tag') != None):
                    _tag = str(i.get('tag'))
                if(i.get('stream') != None):
                    _stream = self.StringToBool(i.get('stream'))
                if(i.get('insecure_registry') != None):
                    _insecure_registry = self.StringToBool(i.get('insecure_registry'))
                if(i.get('auth_config') != None):
                    _auth_config = ast.literal_eval(str(i.get('auth_config')))
        except KeyError:
            print 'no opt'
        try:
            ExecCode = json.dumps({'pull': str( 'Pulling ' + str(_repository) + ':' + str(_tag))})
            self.Socket.Envoi(ExecCode)
            frequence = 10
            index = 0
            for line in self.cli.pull(repository=_repository, tag=_tag, stream=_stream, insecure_registry=_insecure_registry, auth_config=_auth_config):
                ExecCode = json.dumps(json.loads(line), indent=4)
                if _stream == True:
                    index = index +1
                    if(index >= frequence):
                        index = 0
                        self.Socket.Envoi(json.dumps({'pull': str(ExecCode)}))
        except KeyError as e:
            ExecCode = json.dumps({'pull' : str(e)})
            ExecCode = json.dumps({'ERROR': str(ExecCode)})
            return ExecCode
        ExecCode = json.dumps({'pull': str(ExecCode)})
        print 'Pull : ' + ExecCode
        return ExecCode

    def end(self, param):
        return 'end'

    def other(self, param):
        print 'other : ' + str(param)

    def ErreurMsg(self, param):
        print 'Error JSON : ' + str(param)

    def RienRecu(self, param):
        print 'rien : ' + str(param)
        return
    
    def SetPubKey(self, param):
        self.Socket.SetPubKey(param.get('option'))
        return

    def Execute(self, argument):
        switcher = {
            'showcontainer': self.showcontainer,
            'showimage': self.Showimage,
            'showmac': self.Showmac,
            'deleteimage': self.Deleteimage,
            'stopcontainer': self.StopContainer,
            'deletecontainer': self.DeleteContainer,
            'login': self.Login,
            'pull': self.Pull,
            'run': self.run,
            'end': self.end,
            'other': self.other,
            'pubkey': self.SetPubKey,
            'fail': self.ErreurMsg,
            'None': self.RienRecu,
        }
        if(argument != None):
            # try:
                TypeCommande = ''
                try:
                    RequestData = json.loads(argument)
                    TypeCommande = str(RequestData['type'])
                except (KeyError, ValueError) as e:  # includes simplejson.decoder.JSONDecodeError
                    TypeCommande = 'fail'
                    RequestData = e
                return switcher.get(TypeCommande, self.other)(RequestData)#Call the good function according the command, if command not found, call other function
            #except TypeError:
                #    print 'Type Error'
                #    switcher.get('fail','other')(RequestData)#Call the good function according the command, if command not found, call other function
        else:
            self.RienRecu('')
