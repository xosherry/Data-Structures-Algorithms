from __future__ import print_function
import sys
import math
import bisect
from operator import itemgetter
import re
from collections import defaultdict
import time


class Interval:
    def __init__(self, s, f, p):
        self.start = s
        self.finish = f
        self.payoff = p

    def __getitem__(self, item):
        return (self.start, self.finish, self.payoff)[item]

    def __repr__(self):
        return str(self.start) + " " + str(self.finish) + " " + str(self.payoff)


intervals = []
# keys: index corresponding to interval, values: max_payoff
max_payoffs = {}
# keys: index corresponding to interval, values: array of intervals that led to payoff @ that interval key
payoff_chain = defaultdict(list)

#included_intervals = []
included_intervals = {}
bools = []


def sort_by_finish_time(list_of_intervals):
    list_of_intervals.sort(key=itemgetter(1))

    # print("Sorted:")
    # for i in range(len(intervals)):
    #     print(intervals[i].start, intervals[i].finish, intervals[i].payoff)

def get_input():
    user_input = sys.stdin.readlines()

    for line in user_input:
        line = [int(x) for x in line.split() if x.isdigit()]
        if len(line) == 3 and line[0] < line[1]:
            intervals.append(Interval(line[0], line[1], line[2]))

def find_last_non_conflict(finish_times, current_index):
    # bisect_right: finds rightmost value less than or equal to j
    # returns -1 if all previous intervals conflict with interval j
    return bisect.bisect_right(finish_times, intervals[current_index].start) - 1

def find_all_max_payoffs():
    # finds optimal solution considering intervals[0, 1, ..., index]
    #x = time.time()
    finish_times = [i.finish for i in intervals]
    #y = time.time()
    #print("finish_times creation:", y-x)


    max_payoffs[-1] = 0
    payoff_chain[-1] = []

    # a = time.time()
    # c = a # initialize c and d
    # d = a
    for j in range(len(intervals)):
        last_non_conflict = find_last_non_conflict(finish_times, j)
        with_j = intervals[j].payoff + max_payoffs[last_non_conflict]
        without_j = max_payoffs[j-1]
        max_payoffs[j] = max(with_j, without_j)

        # c = time.time()
        # if max_payoffs[j] == with_j:
        #     payoff_chain[j] = [intervals[j]]
        #     #last_non_conflict == -1 when all intervals before j conflicts with it, when not -1 there's some itnervals that fit before
        #     if last_non_conflict is not -1:
        #         payoff_chain[j] = payoff_chain[j] + payoff_chain[last_non_conflict]
        # else:
        #     payoff_chain[j] = payoff_chain[j-1]

        # if max_payoffs[j] == with_j:
        #     last = len(included_intervals) - 1
        #     if last == -1:
        #         included_intervals.append(j)
        #     elif intervals[j].start != intervals[included_intervals[last]].start and intervals[j].finish != intervals[included_intervals[last]].finish:
        #         included_intervals.append(j)
        #     else:
        #         continue

        # if max_payoffs[j] == with_j:
        #     if len(included_intervals) == 0:
        #         included_intervals.append(j)
        #
        #     else:
        #         last = len(included_intervals) - 1
        #         if intervals[j].start == intervals[included_intervals[last]].start \
        #                 and intervals[j].finish == intervals[included_intervals[last]].finish:
        #             included_intervals[last] = j
        #         else:
        #             if intervals[j].start != intervals[included_intervals[last]].start and intervals[j].finish != \
        #                     intervals[included_intervals[last]].finish:
        #                 included_intervals.append(j)

        # if max_payoffs[j] == with_j:
        #     included_intervals.append(j)

        if max_payoffs[j] == with_j:
            bools.append(1)
            #index of the last interval that doesnt conflict and has the biggest payoff
            #included_intervals[j] = last_non_conflict
            included_intervals[j] = last_non_conflict
        else:
            included_intervals[j] = j-1
            bools.append(0)

    #     d = time.time()
    #
    # b = time.time()
    # print("for loop of dp decision:", b-a)
    # print("keeping track:", d-c)





def main():
    get_input()
    sort_by_finish_time(intervals)
    # for i in range(len(intervals)):
    #     print(intervals[i].start, intervals[i].finish, intervals[i].payoff)

    find_all_max_payoffs()

    highest_payoff = max(max_payoffs.values(), key=int)

    print("Maximum Payoff:", highest_payoff)

    # only need to return payoff chain for one of the optimal solutions
    # for k, v in max_payoffs.items():
    #     if v == highest_payoff:
    #         # Intervals must also be sorted from earliest to latest.
    #         sort_by_finish_time(payoff_chain[k])
    #         print(*payoff_chain[k], sep='\n')
    #         break;

    sum = 0
    # for val in included_intervals:
    #     print(intervals[val])
    #     sum += intervals[val].payoff
    # print("sum", sum)

    # result = []
    # result.append(Interval(-1, -1, -1))
    # for i in reversed(included_intervals):
    #     last = len(result) - 1
    #     if intervals[i].start != result[last].start and intervals[i].finish != result[last].finish:
    #         result.append(intervals[i])
    #
    # del result[0]
    #
    # for interval in reversed(result):
    #     print(interval)
    #     sum += interval.payoff
    #
    # print("sum", sum)

    #all_keys = sorted(included_intervals.keys())
    #last_key = the index with the largest payoff, the most recently added

    #print("max payoffs:")
    #print(max_payoffs)
    for k, v in max_payoffs.items():
        if v == highest_payoff:
            last_key = k
            break;

    # for i in intervals:
    #     print(i)

    # print(included_intervals)

    result = []
    while last_key != -1:
        #print('last_key', last_key)

        #print('max_payoffs', max_payoffs[last_key])
        #print('max_payoffs[included_intervals[last_key]]', max_payoffs[included_intervals[last_key]])
        # if max_payoffs[last_key] <= max_payoffs[included_intervals[last_key]]:
        #     result.append(intervals[included_intervals[last_key]])
        #
        # else:
        if bools[last_key] == 1:
            result.append(intervals[last_key])

        last_key = included_intervals[last_key]

    # for i in range(len(bools)):
    #     if bools[i] == 1:
    #         print(intervals[i])



    #find max item
    # for k in included_intervals.keys():
    #     print('k', k)
    #     if k == last_key:
    #         break;
    #     else:
    #         if max_payoffs[k] > max_payoffs[last_key]:
    #             result[len(result) - 1] = intervals[k]

    #replace last item with max item
    #del result[len(result) - 1]

    for i in reversed(result):
        print(i)

    #print(intervals[included_intervals[last_key]])



main()