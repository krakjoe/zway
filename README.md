ZWay
====
*Someone who knows exactly what they are talking about should write this ...*

```php
namespace ZWay {

	class ZWLog {
		public function __construct(int $level = ZWLOG::DEBUG);
		/*
		* @throws RuntimeException
		*/
		public function __construct(string $file, string $mode, int $level = ZWLOG::DEBUG);

		public function error(string $source, int $level, string $message, int $errno = 0);

		const DEBUG;
		const INFO;
		const WARN;
		const ERROR;
		const CRITICAL;
		const SILENT;
	}

	class ZWay {
		/*
		* @throws RuntimeException
		*/
		public function __construct(string $port, string $cfolder, string $tfolder, string $zfolder, string $name, ZWLog $logger);
		
		public function getPort() : string;
		public function getConfigFolder() : string;
		public function getTranslationsFolder() : string;
		public function getZDDXFolder() : string;
		public function getName() : string;
		public function getLogger() ZWLog;

		/*
		* @throws RuntimeException
		*/
		public function discover() : bool;

		public function isIdle() : bool;
		public function isRunning() : bool;

		/*
		* @throws RuntimeException
		*/
		public function start() : bool;

		/*
		* @throws RuntimeException
		*/
		public function stop() : bool;
	}
}
```

