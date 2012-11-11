class xwlBuilder(Builder): 
	def setup(self, platform, builder):
		p = Project( name="xwl" )
		builder.addProject( p )

	def config(self, builder, driver, project, args):
		libdir = "lib/%s/%s" % (args['architecture'], args['configuration'])

		if args['platform'] is LINUX:
			items = ['lib']
			items.append( args['build_architecture'] )
			items.append( args['configuration'] )
			libdir = '/'.join( items )

		builder.includes = ['include']
		builder.libs = [ project.name ]
		builder.setOutput( path=libdir, name=project.name, type=Builder.StaticLibrary )

		driver.config = (args['configuration'].lower() + Premake4.archmap[ args['platform'] ][ args['build_architecture'] ])

	def generate(self, builder):
		premake = Premake4( action=builder.premake_action )
		premake.run()

	def postclean(self, driver, args):
		d = Delete(path=os.path.join(args['libpath'], os.path.pardir), directory=True)
		d.run()