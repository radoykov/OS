import time

def experiment(nums):
    s = 0
    for x in nums:
        if x < 0:   # condition
            s += 1
    return s

# read from file
with open(r"C:\Users\alexr\Projects\OS\CPU\branchPredictor\numbers.txt") as f:
    nums = [int(x) for x in f.read().split()]


sorted_nums = sorted(nums)

import random
shuffled_nums = nums[:]
random.shuffle(shuffled_nums)

t1 = time.perf_counter()
experiment(sorted_nums)
t2 = time.perf_counter()

t3 = time.perf_counter()
experiment(shuffled_nums)
t4 = time.perf_counter()

print("Sorted time:", t2 - t1)
print("Shuffled time:", t4 - t3)

# Generate file with shuffled_nums
# import random

# nums = list(range(1, 5001))
# random.shuffle(nums)

# with open("numbers_shuffled.txt", "w") as f:
#     for x in nums:
#         f.write(str(x) + " ")