
Climsg - A simple System V message queue system.

Climsg is a simple inter-process communication program, mostly designed for scripts that don't usually have such a thing.

Messages can be sent, received, repeated or flushed from the queue, the message system is handled by the kernel, so any modern kernel should be fine, if you have compiled your own kernel you may have to 'opt in' the message system.
Messages can use the default 'key' or can specify a custom 'key' allowing for semi-private  communications, also message 'types' can be  used, a type is  any integer > 0, only messages of the correct type will be peeled of from the queue and printed to stdout, type 0 is a special case and is use to receive any  message regardless of 'type' of course the 'key' must match as well.

Because the messages are handled by the kernel, messages can be passed to and received back from a chroot environment.

Queues are FIFO and LOCAL only.

BUGS etc.
kdhedger68713@gmail.com