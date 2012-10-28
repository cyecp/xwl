class xwlBuilder(Builder): 
	def config(self, platform, builder, defaults):
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
