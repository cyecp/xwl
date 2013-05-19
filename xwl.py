class xwlBuilder(Builder): 
	def setup(self, *args, **kwargs):
		builder = kwargs.get( "builder", None )
		p = Project( name="xwl" )
		builder.addProject( p )

	def config(self, *args, **kwargs):
		host_platform = kwargs.get( "host_platform", None )
		target_platform = kwargs.get( "target_platform", host_platform )
		driver = kwargs.get( "driver", None )
		builder = kwargs.get( "builder", None )
		project = kwargs.get( "project", None )
		params = kwargs.get( "args", None )

		if target_platform is LINUX:
			items = ['lib']
			items.append( params['build_architecture'] )
			items.append( params['configuration'] )
			libdir = '/'.join( items )
		elif target_platform is WINDOWS:
			libdir = "lib/%s/%s" % (params['build_architecture'], params['configuration'])
			#builder.setOutput( path=libdir, name=project.name, type=Builder.StaticLibrary )
		else:
			libdir = "lib/%s/%s" % (params['architecture'], params['configuration'])

		builder.includes = ['include']
		builder.libs = [ project.name ]
		builder.setOutput( path=libdir, name=project.name, type=Builder.DynamicLibrary )

		driver.config = (params['configuration'].lower() + Premake4.archmap[ params['platform'] ][ params['build_architecture'] ])

	def generate(self, *args, **kwargs):
		builder = kwargs.get( "builder", None )
		premake = Premake4( action=builder.premake_action )
		premake.run()

	def postclean(self, *args, **kwargs):
		params = kwargs.get( "args", None )
		d = Delete(path=os.path.join(params['libpath'], os.path.pardir), directory=True)
		d.run()