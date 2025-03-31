from typing import List, Tuple

from mapUtil import (
    CityMap,
    computeDistance,
    createUSTCMap,
    createHefeiMap,
    locationFromTag,
    makeTag,
)
from util import Heuristic, SearchProblem, State, UniformCostSearch

# BEGIN_YOUR_CODE (You may add some codes here to assist your coding below if you want, but don't worry if you deviate from this.)

# END_YOUR_CODE

# *IMPORTANT* :: A key part of this assignment is figuring out how to model states
# effectively. We've defined a class `State` to help you think through this, with a
# field called `memory`.
#
# As you implement the different types of search problems below, think about what
# `memory` should contain to enable efficient search!
#   > Check out the docstring for `State` in `util.py` for more details and code.

########################################################################################
# Problem 1a: Modeling the Shortest Path Problem.


class ShortestPathProblem(SearchProblem):
    """
    Defines a search problem that corresponds to finding the shortest path
    from `startLocation` to any location with the specified `endTag`.
    """

    def __init__(self, startLocation: str, endTag: str, cityMap: CityMap):
        self.startLocation = startLocation
        self.endTag = endTag
        self.cityMap = cityMap

    def startState(self) -> State:
        # BEGIN_YOUR_CODE (our solution is 1 line of code, but don't worry if you deviate from this)
        # 本问题中不需要设置State.memory，统一设为None
        # 起始状态：location为startLocation
        return State(self.startLocation,None)
        # END_YOUR_CODE

    def isEnd(self, state: State) -> bool:
        # BEGIN_YOUR_CODE (our solution is 1 line of code, but don't worry if you deviate from this)
        # 判断结束：当前location的Tag中包含endTag
        return self.endTag in self.cityMap.tags[state.location]
        # END_YOUR_CODE

    def successorsAndCosts(self, state: State) -> List[Tuple[str, State, float]]:
        # BEGIN_YOUR_CODE (our solution is 4 lines of code, but don't worry if you deviate from this)
        # 返回一个list，包含与当前location相邻的location（即下一步行动action）、对应的新State和边的cost
        # 构成的tuple
        SACs = []
        for action, cost in self.cityMap.distances[state.location].items():
            SACs.append((action, State(action, None), cost))
        return SACs
        # END_YOUR_CODE


########################################################################################
# Problem 1b: Custom -- Plan a Route through USTC


def getUSTCShortestPathProblem() -> ShortestPathProblem:
    """
    Create your own search problem using the map of USTC, specifying your own
    `startLocation`/`endTag`.

    Run `python mapUtil.py > readableUSTCMap.txt` to dump a file with a list of
    locations and associated tags; you might find it useful to search for the following
    tag keys (amongst others):
        - `landmark=` - Hand-defined landmarks (from `data/USTC-landmarks.json`)
        - `amenity=`  - Various amenity types (e.g., "coffee", "food")
    """
    cityMap = createUSTCMap()

    # BEGIN_YOUR_CODE (our solution is 2 lines of code, but don't worry if you deviate from this)
    # 从老北门出发到西区图书馆
    startLocation = locationFromTag(makeTag('landmark','the_original_north_gate'),cityMap)
    endTag = makeTag('landmark','west_campus_library')
    # END_YOUR_CODE
    return ShortestPathProblem(startLocation, endTag, cityMap)


########################################################################################
# Problem 2a: Modeling the Waypoints Shortest Path Problem.


class WaypointsShortestPathProblem(SearchProblem):
    """
    Defines a search problem that corresponds to finding the shortest path from
    `startLocation` to any location with the specified `endTag` such that the path also
    traverses locations that cover the set of tags in `waypointTags`.

    Think carefully about what `memory` representation your States should have!
    """
    def __init__(
        self, startLocation: str, waypointTags: List[str], endTag: str, cityMap: CityMap
    ):
        self.startLocation = startLocation
        self.endTag = endTag
        self.cityMap = cityMap

        # We want waypointTags to be consistent/canonical (sorted) and hashable (tuple)
        self.waypointTags = tuple(sorted(waypointTags))

    def startState(self) -> State:
        # BEGIN_YOUR_CODE (our solution is 1 line of code, but don't worry if you deviate from this)
        # 定义State.memory为当前未经过的waypointTags构成的tuple，满足memory可哈希的要求
        # 起始状态：location为startLocation，memory为不包括起始点tag的waypointTags构成的tuple
        return State(self.startLocation, tuple(tags for tags in self.waypointTags if tags not in self.cityMap.tags[self.startLocation]))
        # END_YOUR_CODE

    def isEnd(self, state: State) -> bool:
        # BEGIN_YOUR_CODE (our solution is 1 lines of code, but don't worry if you deviate from this)
        # 判断结束：当前location的Tag中包含endTag，且memory为空
        return self.endTag in self.cityMap.tags[state.location] and not state.memory
        # END_YOUR_CODE

    def successorsAndCosts(self, state: State) -> List[Tuple[str, State, float]]:
        # BEGIN_YOUR_CODE (our solution is 13 lines of code, but don't worry if you deviate from this)
        # 返回一个list，包含与当前location相邻的location（即下一步行动action）、对应的新State和边的cost
        # 构成的tuple，新State.memory需去掉action的tags
        # 采用遍历上一个State的memory的方式可以让tags的顺序保持一致，确保同样的location和tags对应同一个State
        SACs = []
        RemainingTags = state.memory
        for action, cost in self.cityMap.distances[state.location].items():
            NewRemainTags = tuple(tags for tags in RemainingTags if tags not in self.cityMap.tags[action])
            SACs.append((action, State(action,NewRemainTags), cost))
        return SACs
        # END_YOUR_CODE


########################################################################################
# Problem 2b: Custom -- Plan a Route with Unordered Waypoints through USTC


def getUSTCWaypointsShortestPathProblem() -> WaypointsShortestPathProblem:
    """
    Create your own search problem using the map of USTC, specifying your own
    `startLocation`/`waypointTags`/`endTag`.

    Similar to Problem 1b, use `readableUSTCMap.txt` to identify potential
    locations and tags.
    """
    cityMap = createUSTCMap()
    # BEGIN_YOUR_CODE (our solution is 3 lines of code, but don't worry if you deviate from this)
    # 从老北门出发到西区图书馆，经过咖啡厅
    startLocation = locationFromTag(makeTag('landmark','the_original_north_gate'),cityMap)
    waypointTags = [makeTag('amenity','coffee')]
    endTag = makeTag('landmark','west_campus_library')
    # END_YOUR_CODE
    return WaypointsShortestPathProblem(startLocation, waypointTags, endTag, cityMap)


########################################################################################
# Problem 3a: A* to UCS reduction

# Turn an existing SearchProblem (`problem`) you are trying to solve with a
# Heuristic (`heuristic`) into a new SearchProblem (`newSearchProblem`), such
# that running uniform cost search on `newSearchProblem` is equivalent to
# running A* on `problem` subject to `heuristic`.
#
# This process of translating a model of a problem + extra constraints into a
# new instance of the same problem is called a reduction; it's a powerful tool
# for writing down "new" models in a language we're already familiar with.


def aStarReduction(problem: SearchProblem, heuristic: Heuristic) -> SearchProblem:
    class NewSearchProblem(SearchProblem):
        def __init__(self):
            # BEGIN_YOUR_CODE (our solution is 3 line of code, but don't worry if you deviate from this)
            # 三个变量均与UCS问题的变量保持一致
            self.startLocation = problem.startLocation
            self.endTag = problem.endTag
            self.cityMap = problem.cityMap
            # END_YOUR_CODE

        def startState(self) -> State:
            # BEGIN_YOUR_CODE (our solution is 1 line of code, but don't worry if you deviate from this)
            # 起始状态与UCS问题的起始状态相同
            return problem.startState()
            # END_YOUR_CODE

        def isEnd(self, state: State) -> bool:
            # BEGIN_YOUR_CODE (our solution is 1 line of code, but don't worry if you deviate from this)
            # 结束条件与UCS问题的结束条件相同
            return problem.isEnd(state)
            # END_YOUR_CODE

        def successorsAndCosts(self, state: State) -> List[Tuple[str, State, float]]:
            # BEGIN_YOUR_CODE (our solution is 7 lines of code, but don't worry if you deviate from this)
            # A*算法中cost的计算公式为cost'(s,a) = cost(s,a) + h(Succ(s,a)) - h(s)
            # h()为启发函数，即heuristic.evaluate()
            SACs = problem.successorsAndCosts(state)
            SACsForAStar = []
            for action, NextState, cost in SACs:
                CostForAStar = cost + heuristic.evaluate(NextState) - heuristic.evaluate(state)
                SACsForAStar.append((action,NextState,CostForAStar))
            return SACsForAStar
            # END_YOUR_CODE

    return NewSearchProblem()


########################################################################################
# Problem 3c: "straight-line" heuristic for A*


class StraightLineHeuristic(Heuristic):
    """
    Estimate the cost between locations as the straight-line distance.
        > Hint: you might consider using `computeDistance` defined in `mapUtil.py`
    """
    def __init__(self, endTag: str, cityMap: CityMap):
        self.endTag = endTag
        self.cityMap = cityMap

        # Precompute
        # BEGIN_YOUR_CODE (our solution is 4 lines of code, but don't worry if you deviate from this)
        # 提前计算好所有终点的GeoLocation，方便后续计算
        possibleLocations = sorted(
        [location for location, tags in cityMap.tags.items() if endTag in tags]
        )
        self.endGeoLocation = [self.cityMap.geoLocations[locations] for locations in possibleLocations]
        # END_YOUR_CODE

    def evaluate(self, state: State) -> float:
        # BEGIN_YOUR_CODE (our solution is 6 lines of code, but don't worry if you deviate from this)
        # 计算当前GeoLocation
        NowGeoLocation = self.cityMap.geoLocations[state.location]
        return min([computeDistance(NowGeoLocation, ends) for ends in self.endGeoLocation])
        # END_YOUR_CODE


########################################################################################
# Problem 3e: "no waypoints" heuristic for A*


class NoWaypointsHeuristic(Heuristic):
    """
    Returns the minimum distance from `startLocation` to any location with `endTag`,
    ignoring all waypoints.
    """
    def __init__(self, endTag: str, cityMap: CityMap):
        # Precompute
        # BEGIN_YOUR_CODE (our solution is 14 lines of code, but don't worry if you deviate from this)
        # 用UCS算法得到所有可能终点到地图上各点间的最短距离，再取每一点和最近的终点间的距离建立索引表
        # evaluate方法的时间复杂度为O(1)（即直接查找）
        # 先寻找所有可能的终点
        possibleLocations = sorted(
        [location for location, tags in cityMap.tags.items() if endTag in tags]
        )
        # self.Distances = Dict[str:float]标记location和最近的终点间的距离
        self.Distances = {}
        for locations in possibleLocations:
            # ShortestPathProblem的实例中把endTag设为不存在的Tag，即可保证UCS算法搜索完整张图
            problem = ShortestPathProblem(locations,"NoEnd",cityMap)
            ucs = UniformCostSearch(verbose=0)
            ucs.solve(problem)
            # 更新self.Distances
            for states, distances in ucs.pastCosts.items():
                actions = states.location
                if actions not in self.Distances or self.Distances[actions] > distances:
                    self.Distances[actions] = distances
        # END_YOUR_CODE

    def evaluate(self, state: State) -> float:
        # BEGIN_YOUR_CODE (our solution is 1 line of code, but don't worry if you deviate from this)
        # 直接取索引表中对应内容
        return self.Distances[state.location]
        # END_YOUR_CODE


########################################################################################
# Problem 3f: Plan a Route through Hefei with or without a Heuristic

def getHefeiShortestPathProblem(cityMap: CityMap) -> ShortestPathProblem:
    """
    Create a search problem using the map of Hefei
    """
    startLocation=locationFromTag(makeTag("landmark", "USTC"), cityMap)
    endTag=makeTag("landmark", "Chaohu")
    # BEGIN_YOUR_CODE (our solution is 1 lines of code, but don't worry if you deviate from this)
    return ShortestPathProblem(startLocation, endTag, cityMap)
    # END_YOUR_CODE

def getHefeiShortestPathProblem_withHeuristic(cityMap: CityMap) -> ShortestPathProblem:
    """
    Create a search problem with Heuristic using the map of Hefei
    """
    startLocation=locationFromTag(makeTag("landmark", "USTC"), cityMap)
    endTag=makeTag("landmark", "Chaohu")
    # BEGIN_YOUR_CODE (our solution is 2 lines of code, but don't worry if you deviate from this)
    problem = aStarReduction(ShortestPathProblem(startLocation, endTag, cityMap),StraightLineHeuristic(endTag, cityMap))
    return  problem
    # END_YOUR_CODE
