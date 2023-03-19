from faker import Faker
from random import randint
import cjson
import ujson
import time

def generate_data(amount):
	faker = Faker()
	data = []
	for _ in range(amount):
		key = faker.name()
		way = randint(1, 3)
		if way == 1:
			val = faker.address()
		elif way == 2:
			val = faker.text()
		else:
			val = randint(1, 10**9)
		data.append(f'"{key}": {val}' if way == 3 else f'"{key}": "{val}"')
		break
	return '{' + ','.join(data) + '}'


class Timer:
	def __init__(self):
		self.start = time.time()

	def log(self):
		end = time.time()
		print("The time of execution is :", (end-self.start) * 10**3, "ms")

	def reset(self):
		self.start = time.time()


timer = Timer()
json_str = generate_data(100000)

# ujson
print('ujson')

print('loads', end='\t-\t')
timer.reset()
doc = ujson.loads(json_str)
timer.log()

print('dumps', end='\t-\t')
timer.reset()
ujson.dumps(doc)
timer.log()


# cjson
print('cjson')

print('loads', end='\t-\t')
timer.reset()
doc = cjson.loads(json_str)
timer.log()

print('dumps', end='\t-\t')
timer.reset()
cjson.dumps(doc)
timer.log()



