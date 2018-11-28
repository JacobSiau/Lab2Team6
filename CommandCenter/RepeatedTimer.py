# source: https://stackoverflow.com/questions/474528/what-is-the-best-way-to-repeatedly-execute-a-function-every-x-seconds-in-python
from threading import Timer


class RepeatedTimer(object):

    def __init__(self, interval, task):
        self._timer = None
        self.interval = interval
        self.task = task
        self.is_running = False
        self.start()

    def _run(self):
        self.is_running = False
        self.start()
        self.task()

    def start(self):
        if not self.is_running:
            self._timer = Timer(self.interval, self._run)
            self._timer.start()
            self.is_running = True

    def stop(self):
        self._timer.cancel()
        self.is_running = False
