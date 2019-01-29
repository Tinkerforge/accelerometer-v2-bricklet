use std::{error::Error, io};

use tinkerforge::{accelerometer_v2_bricklet::*, ip_connection::IpConnection};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Accelerometer Bricklet 2.0.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let a = AccelerometerV2Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    // Get current acceleration.
    let acceleration = a.get_acceleration().recv()?;

    println!("Acceleration [X]: {} g", acceleration.x as f32 / 10000.0);
    println!("Acceleration [Y]: {} g", acceleration.y as f32 / 10000.0);
    println!("Acceleration [Z]: {} g", acceleration.z as f32 / 10000.0);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
