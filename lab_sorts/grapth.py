from matplotlib import pyplot

bubble_y=[]
with open('bubble_o0.csv') as f:
    for l in f:
        bubble_y.append(float(l))
bubble_x = [i for i in range(1, len(bubble_y)+1)]

gnom_y=[]
with open('bubble_o1.csv') as f:
    for l in f:
        gnom_y.append(float(l))
gnom_x = [i for i in range(1, len(gnom_y)+1)]

insert_y=[]
with open('bubble_o2.csv') as f:
    for l in f:
        insert_y.append(float(l))
insert_x = [i for i in range(1, len(insert_y)+1)]

heap_y=[]
with open('bubble_o3.csv') as f:
    for l in f:
        heap_y.append(float(l))
heap_x = [i for i in range(1, len(heap_y)+1)]

# insert2_y=[]
# with open('bubble_o3.csv') as f:
#     for l in f:
#         insert2_y.append(float(l))
# insert2_x = [i for i in range(1, len(insert2_y)+1)]

fig, ax = pyplot.subplots()
ax.plot(bubble_x, bubble_y, label="O0")
ax.plot(gnom_x, gnom_y, label="O1")
ax.plot(insert_x, insert_y, label="O2")
ax.plot(heap_x, heap_y, label="O3")
# ax.semilogy()
# ax.plot(insert2_x, insert2_y, label="O3")
# pyplot.plot(bubble_x, bubble_y, 'y', gnom_x, gnom_y, 'g', insert_x, insert_y, 'b', insert2_x, insert2_y, 'r')
# pyplot.plot(bubble_x, bubble_y, 'b', gnom_x, gnom_y, 'r')
# ax = pyplot.gca()
# ax.set_yscale('log')
ax.legend()
pyplot.show()