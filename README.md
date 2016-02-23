ZWay
====
*Someone who knows exactly what they are talking about should write this ...*

Installation
===========

First the includes for PHP must be patched:

  - Open ```main/php.h```
  - Search for ```int Debug```
  - Comment out the line
  - Save and close file

*This is quite horrible, I'm not sure why the symbol is defined in PHP ... but it is, and in libzway*

Then the extension can be built and installed:

    $ git clone https://github.com/krakjoe/zway
    $ cd zway
    $ phpize
    $ ./configure --with-zway=/path/to/z-way-server-dist
    $ make
    $ sudo make install

*```/path/to/z-way-server-dist``` should contain the directories ```libzway```, and ```libs```, and the executable ```z-way-server```.*

Then ... profit ?

API
===
*There is WIP here ... don't look, unless extremely brave*

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

