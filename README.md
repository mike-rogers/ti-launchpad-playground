# Mike's TI LaunchPad PlayGround

Have you heard about [TI](http://www.ti.com/)'s MSP430-based [LaunchPad](http://www.ti.com/launchpad)?

You can buy them from (prices and availability may change depending on time and shipping address):

* [TI directly](https://estore.ti.com/MSP-EXP430G2-MSP430-LaunchPad-Value-Line-Development-kit-P2031.aspx) for $10 USD
* [Amazon.com](http://www.amazon.com/Texas-Instruments-Educational-Products-MSP-EXP430G2/dp/B004G52S82) for $14.95 USD

I'm a huge fan. I had to use the MSP430 architecture for a project recently and it was a blast. It's such a low-powered micro that you can put it to sleep for ten years on a single AA battery. That's nuts.

## What am I trying to do?

My intent with this repository is to offer some code to help get started with some of the on-board peripherals and provide an example for some off-board peripherals accessed through SPI or GPIO. There are a couple of directions I may take this repository:

* A camera that I can attach to a kite and take aerial snapshots around my house.
* A front-end to a PCB UV curing box.
* An 802.15.4-enabled sensor device.

## What peripherals are supported?

Currently none. I will update this area as support is added for a peripheral.

## What's all this code?

I'm attempting to write this code in a TDD fashion. If you're unfamiliar with embedded TDD (or are otherwise curious about it), I cannot recommend [Test Driven Development for Embedded C](http://pragprog.com/book/jgade/test-driven-development-for-embedded-c) by James W. Grenning highly enough. Some of the tools he used in that book have evolved slightly, but the theory is very sound.

The tool I'm getting the most use out of is [Ceedling](http://throwtheswitch.org/white-papers/ceedling-intro.html), which is a Rake-based build/test/generating system for embedded C projects. Under the hood, Ceedling makes use of:

* [Unity](http://throwtheswitch.org/white-papers/unity-intro.html), a unit testing framework in C
* [CMock](http://throwtheswitch.org/white-papers/cmock-intro.html), a mocking framework in C
* [CException](http://throwtheswitch.org/white-papers/cexception-intro.html), a framework for simple exception handling in C

[Ceedling](https://github.com/ThrowTheSwitch/Ceedling) ties all of these frameworks together in a neat, beautiful package.

Additionally the comments are done in the style of [Doxygen](http://www.stack.nl/~dimitri/doxygen/), so if you have it installed you can invoke it with:

```
doxygen doxygen.config
```

The output will be in `docs/doxygen`.

## What do I need to start?

You will need:

* Ruby (I'm using version 1.9.2)
* Ceedling (follow the instructions from the links above)
* GCC (all tests are compiled with your host architecture)
* MSP430 environment for your OS
 * [osx-launchpad](http://code.google.com/p/osx-launchpad/) for Mac
 * [MSPGCC](http://sourceforge.net/apps/mediawiki/mspgcc/index.php?title=MSPGCC_Wiki) and [MSPDebug](http://mspdebug.sourceforge.net/) for Linux
 * [Code Composer Studio 5.3](http://processors.wiki.ti.com/index.php/Download_CCS) for Windows

_Note_: Currently the cross compiler in the `project.yml` file for Ceedling is tied to `osx-launchpad` - feel free to modify for your own cross compiler.

Once you get everything installed, you can run the tests by command line with the following command:

```
rake test:all
```

If you want to get a list of all commands that Ceedling exposes,

```
rake -T
```

## Anything else?

Feel free to drop me a line. I'll respond if you don't get caught in my spam filter.