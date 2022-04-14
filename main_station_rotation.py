from argparse import Namespace
from asyncio import subprocess
from os import name
from sched import scheduler
from lorettOrbital.orbital import Scheduler
from HardwareLorettRotator import *
from orbital import *
from threading import Thread
from pprint import pprint
from time import sleep
import os
import time


DEBUG = False
class Main_Lorett_Rotator:
    '''Класс адаптер для организации взаимодействия между отдельными компонентами'''

    def __init__(self) -> None:
        self.stationName = 'r8s'

        self.path = 'C:/Users/T_OFFICE_4/Desktop/LEVEL-UP (1)/level-up'
        # self.path = '/home/pi/Desktop/Lorett-Rotator/level-up'

        self.lat = 54.5268
        self.lon = 36.1673
        self.alt = 0.159
        self.timeZone = 3

        self.schedule = []

        self.logger = LorettLogging(self.path)
        try:
            self.orbital = Scheduler(self.stationName, self.lat, self.lon, self.alt, self.path, timeZone=self.timeZone)
            self.logger.info('start lorettOrbital.Scheduler')
        except:
            self.logger.error('no start lorettOrbital.Scheduler')
        self.schedule += self.orbital.getSchedule(24, returnNameSatellite=True, returnTable=False)
        # pprint(self.schedule)
        try:
            self.logger.info('start Rotator_SerialPort')
            self.rotator = Rotator_SerialPort(self.logger, DEBUG=DEBUG, port='COM3')
        except:
            self.logger.error('no start Rotator_SerialPort')


    def tracking(self, track: tuple, nameSatellite) -> object:
        time_tr = (int(track[1][-1][0][0:2])-int(track[1][0][0][0:2]))*3600+(int(track[1][-1][0][3:5])-int(track[1][0][0][3:5]))*60+int(track[1][-1][0][6:8])-int(track[1][0][0][6:8])
        # os.startfile(f'{self.path}/канал2.py')
        #subprocess.Popen(f"sudo python3 SDRReader.py --s {nameSatellite} --t {time_tr}", shell=True)
        '''Функция для отслеживания спутника во время пролета'''
        self.logger.info(f'start tracking satellite')
        pprint(track)
        for steps in track[1][1:]:
            self.logger.debug(f'Go to pozitions: az: {steps[1]} el: {steps[2]}')
            self.rotator.rotate(steps[1], steps[2])
            sleep(3)
            
        self.rotator.homing()
        
  

    def sleep_to_next(self, time_sleep:datetime, nameSatellite:str, timeRecord):
        time_sleep = int(time_sleep.total_seconds())
        self.logger.info(f'Next satellite {nameSatellite} pass to: {time_sleep} seconds')
        while time_sleep > 70:
            sleep(10)
            time_sleep -= 10
            self.logger.debug(f'Next satellite {nameSatellite} pass to: {time_sleep} seconds')
        #todo запустить скрипт записи
        self.recording(nameSatellite, timeRecord)
        while time_sleep > 1:
            sleep(1)
            time_sleep -= 1
            self.logger.debug(f'Next satellite {nameSatellite} pass to: {time_sleep} seconds')

    def recording(self, nameSatellite, timeRecord):
        # os.startfile(f'{self.path}/канал2.py --s {nameSatellite} --t {timeRecord}')
        pass


    def main(self):
        while True:
            #print(self.schedule)
            # берем следующий пролет
            satPas = self.schedule[0]
            self.schedule = self.schedule[1:]
            # вычисляем время до пролета
            sleep_time = satPas[1][0] - datetime.utcnow()
            timeRecord = int((satPas[1][0] - satPas[1][2]).total_seconds())
            self.sleep_to_next(sleep_time, satPas[0], timeRecord)
            self.tracking(self.orbital.nextPass(), satPas[0])
if __name__ == '__main__':
    station = Main_Lorett_Rotator()
    station.main()
