s2e = {
	kleeArgs = {
		--"--use-batching-search",
		--"--batch-time=1.0",
		--"--use-random-path",
		"--use-dfs-search=true",

		--Optimiztions for faster execution
		--"--state-shared-memory",
		"--flush-tbs-on-state-switch=false",

		--Concolic mode
		"--use-concolic-execution=false",
		"--use-fast-cex-solver=false",
	}
}

plugins = {
	"BaseInstructions",
	"RawMonitor",
	--"ModuleExecutionDetector",
	"FunctionMonitor",
	"MemoryManager",
	--"ExecutionTracer",
	--"CallTracer",
	"HostFiles",
}

pluginsConfig = {}

pluginsConfig.RawMonitor = {
	kernelStart  = 0xc0000000,
	--下面的数值是什么不重要
	__kmalloc = {
		delay = false,
		name = "__kmalloc",
		start = 0xc0000000,
		size = 0x10,
		nativebase = 0x80480000,
		kernelmode = true
	}
}

pluginsConfig.MemoryManager = {
	terminateOnBugs = false,
	--new version(from register), change to false
	getParFromStack = false,
	pc___kmalloc = 0xc10faee0,
}

pluginsConfig.HostFiles = {
	baseDirs = {"/home/xyj/research/s2eTestKernel/linux-3.5.4/../tests/syscalls/setfsgid/"},
}
