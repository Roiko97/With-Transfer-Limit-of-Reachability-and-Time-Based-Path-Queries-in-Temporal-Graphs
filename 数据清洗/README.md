# dataWashing
Base of GTFS  http://transitfeeds.com/

Input:
    Line 1:stop#, route#,route_stops#,stop_times#  
           All of stop ( from zero) , route number , sum of stop by route , sum of route#i的车站 × 班次
    Line 2:route id
    Line 3:route#i的车站数
    Line 4:车站id
    Line 5：班次
    Line 6 ~ ： Arrival TIME， Start TIME

Output：
  结点编号  ， 出边邻居的个数 ， 出边邻居的id ， 有多少条边去邻居点#i ， 旅程所花费的时间  ， 图中的边权值（出发时间）
  车辆唯一id
