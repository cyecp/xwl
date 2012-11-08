class xwlBuilder(Builder): 
	def config(self, platform, builder):
		builder.includes = ['include']
		builder.libs = ['xwl']
		builder.projects = ['xwl']
		builder.targets = ['xwl']

	def generate(self, builder):
		premake = Premake4( action=builder.premake_action )
		premake.run()

	def prebuild(self, driver, args):
		md = Makedirs(path=args['libpath'])
		md.run()

		args['config'] = args['configuration'].lower() + args['architecture']

	def postclean(self, driver, args):
		d = Delete(path=args['libpath'], directory=True)
		d.run()

	def outputs(self, platform, builder, driver, args):
		outputs = []

		#builder.addOutput( name="xwl", type="static" )

		if platform is LINUX:
			outputs.append( "libxwl.a" )
		elif platform is MACOSX:
			outputs.append( "libxwl.a" )
		elif platform is WINDOWS:
			outputs.append( "xwl.lib" )

		return outputs