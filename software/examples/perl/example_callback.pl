#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletAccelerometerV2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Accelerometer Bricklet 2.0

# Callback subroutine for acceleration callback
sub cb_acceleration
{
    my ($x, $y, $z) = @_;

    print "Acceleration [X]: " . $x/10000.0 . " g\n";
    print "Acceleration [Y]: " . $y/10000.0 . " g\n";
    print "Acceleration [Z]: " . $z/10000.0 . " g\n";
    print "\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $a = Tinkerforge::BrickletAccelerometerV2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register acceleration callback to subroutine cb_acceleration
$a->register_callback($a->CALLBACK_ACCELERATION, 'cb_acceleration');

# Set period for acceleration callback to 1s (1000ms)
$a->set_acceleration_callback_configuration(1000, 0);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
