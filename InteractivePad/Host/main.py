# (c) Sergey Kostyuk, 2017
# License: MIT
# Basis: https://github.com/dot-cat/dotcat_platform/blob/v0.01/actors/actor_serial.py

import pyautogui

import serial
from threading import Thread, Event


class ActorSerial(object):
    def __init__(self, tty, baudrate):
        """
        Конструктор. Запускает процесс-слушатель на порту tty
        :param tty: UART-устройство
        :param baudrate: скорость порта
        """

        self.stop_event = Event()
        self.serial = serial.Serial(tty)
        self.serial.baudrate = baudrate
        self.listener_thread = Thread(target=self._listener, daemon=False)
        self.listener_thread.start()

    def __del__(self):
        """
        Деструктор. Освобождает занятые порты, останавливает процессы
        :return: None
        """
        self.stop_event.set()
        self.serial.close()

    def _listener(self):
        """
        Слушатель. Ждет события в консоли и запускает его обработчик
        :return: None
        """
        while not self.stop_event.is_set():
            data = self.serial.readline()
            thread = Thread(target=self._handler, args=(data,), daemon=True)
            thread.start()  # запускаем дочерний поток

    def _handler(self, event):
        """
        Обработчик событий
        :param event: событие, строка
        :return: None
        """
        print('event: {0}'.format(event))

        if   event == b'left\r\n':
            pyautogui.typewrite(['left'])

        elif event == b'right\r\n':
            pyautogui.typewrite(['right'])

        else:
            print('Warning: Unknown event: {0}'.format(event))


actor = ActorSerial("/dev/ttyUSB0", 9600)

