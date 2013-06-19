class xwlRasberryPiBuilder(Builder):
	def setup(*args, **kwargs):
		builder = kwargs.get( "builder", None )
		p = Project( name="xwl" )
		builder.addProject( p )

	def config(*args, **kwargs):
		host_platform = kwargs.get( "host_platform", None )
		target_platform = kwargs.get( "target_platform", host_platform )
		driver = kwargs.get( "driver", None )
		builder = kwargs.get( "builder", None )
		project = kwargs.get( "project", None )
		params = kwargs.get( "args", None )

		libdir = "lib/%s/%s" % (params['architecture'], params['configuration'])

		if target_platform is LINUX:
			items = ['lib']
			items.append( params['build_architecture'] )
			items.append( params['configuration'] )
			libdir = '/'.join( items )

		builder.includes = ['include']
		builder.libs = [ project.name ]
		builder.addOutput( path=libdir, name=project.name, type=Builder.DynamicLibrary )

		driver.config = (params['configuration'].lower() + Premake4.archmap[ params['platform'] ][ params['build_architecture'] ])


	def generate(self, *args, **kwargs):
		builder = kwargs.get( "builder", None )
		premake = Premake4( action=builder.premake_action )
		premake.with_egl = True
		premake.rpi = True

		# until xwl has another way of doing input on RPi, we'll need to at least build with X11.
		premake.with_x11 = True
		premake.run()

	def postclean(*args, **kwargs):
		params = kwargs.get( "args", None )
		d = Delete(path=os.path.join(params['libpath'], os.path.pardir), directory=True)
		d.run()
