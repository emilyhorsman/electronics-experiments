from Adafruit_BluefruitLE.services import UART
import Adafruit_BluefruitLE
import atexit
import click
import os
import pygame
import pygame.joystick as joystick
import time


def list_bluetooth_devices(ble):
    adapter = ble.get_default_adapter()
    adapter.power_on()
    adapter.start_scan()
    atexit.register(adapter.stop_scan)
    print('Searching for Bluetooth Devices:')
    known = set()
    for _ in range(30):
        found = set(UART.find_devices())
        for device in found - known:
            print(device.id, device.name)
        known |= found
        time.sleep(1)
    adapter.stop_scan()


def get_device(ble, id):
    print('Starting', id)
    adapter = ble.get_default_adapter()
    adapter.power_on()
    adapter.start_scan()
    atexit.register(adapter.stop_scan)
    for _ in range(30):
        found = set(UART.find_devices())
        for device in found:
            if str(device.id) == id:
                adapter.stop_scan()
                return device
        time.sleep(1)


@click.command()
@click.option(
    '--list-joysticks',
    is_flag=True,
    help='List available joysticks.'
)
@click.option(
    '--list-bluetooth',
    is_flag=True,
    help='List available UART Bluetooth devices.'
)
@click.option('--joy', type=int)
@click.option('--device')
def app(list_joysticks, list_bluetooth, joy, device):
    if list_joysticks:
        joystick.init()
        print('Joysticks:')
        for i in range(joystick.get_count()):
            j = joystick.Joystick(i)
            print(j.get_id(), j.get_name())

    if list_bluetooth:
        ble = Adafruit_BluefruitLE.get_provider()
        ble.initialize()
        ble.run_mainloop_with(lambda: list_bluetooth_devices(ble))

    if device is not None:
        ble = Adafruit_BluefruitLE.get_provider()
        ble.initialize()
        ble.run_mainloop_with(lambda: get_device(ble, device))
        return
        bluetooth = get_device(ble, device)
        bluetooth.connect()
        atexit.register(bluetooth.disconnect)
        UART.discover(bluetooth)
        uart = UART(bluetooth)

    if joy is not None:
        os.environ['SDL_VIDEODRIVER'] = 'dummy'
        pygame.init()
        joystick.init()
        j = joystick.Joystick(joy)
        j.init()
        print('Axes:', j.get_numaxes())
        while True:
            pygame.event.get()
            for i in range(j.get_numaxes()):
                if bluetooth and uart:
                    uart.write('{}{}\n'.format(i, j.get_axis(i)))

            print(
                '   '.join([str(j.get_axis(i)) for i in range(j.get_numaxes())]),
                end='\r'
            )
            time.sleep(0.05)




if __name__ == '__main__':
    app()
