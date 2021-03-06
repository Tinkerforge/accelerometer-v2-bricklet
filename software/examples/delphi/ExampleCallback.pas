program ExampleCallback;

{$ifdef MSWINDOWS}{$apptype CONSOLE}{$endif}
{$ifdef FPC}{$mode OBJFPC}{$H+}{$endif}

uses
  SysUtils, IPConnection, BrickletAccelerometerV2;

type
  TExample = class
  private
    ipcon: TIPConnection;
    a: TBrickletAccelerometerV2;
  public
    procedure AccelerationCB(sender: TBrickletAccelerometerV2; const x: longint;
                             const y: longint; const z: longint);
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'XYZ'; { Change XYZ to the UID of your Accelerometer Bricklet 2.0 }

var
  e: TExample;

{ Callback procedure for acceleration callback }
procedure TExample.AccelerationCB(sender: TBrickletAccelerometerV2; const x: longint;
                                  const y: longint; const z: longint);
begin
  WriteLn(Format('Acceleration [X]: %f g', [x/10000.0]));
  WriteLn(Format('Acceleration [Y]: %f g', [y/10000.0]));
  WriteLn(Format('Acceleration [Z]: %f g', [z/10000.0]));
  WriteLn('');
end;

procedure TExample.Execute;
begin
  { Create IP connection }
  ipcon := TIPConnection.Create;

  { Create device object }
  a := TBrickletAccelerometerV2.Create(UID, ipcon);

  { Connect to brickd }
  ipcon.Connect(HOST, PORT);
  { Don't use device before ipcon is connected }

  { Register acceleration callback to procedure AccelerationCB }
  a.OnAcceleration := {$ifdef FPC}@{$endif}AccelerationCB;

  { Set period for acceleration callback to 1s (1000ms) }
  a.SetAccelerationCallbackConfiguration(1000, false);

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy; { Calls ipcon.Disconnect internally }
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
