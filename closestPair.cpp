#include <iostream>
#include <iomanip> //for cout.precision
#include <utility>

#include <iostream>
#include <cstdlib>
#include <vector>
#include <cfloat>
#include <cmath>

#include <algorithm>

#include "Helpers.h"

using namespace std;
typedef pair<vector<pair<Point, Point>>, double> closestPairsAndDist;
typedef vector<pair<Point, Point>> closestPairs;



closestPairs brute(vector<Point> inputPoints);

closestPairs basic(vector<Point> inputPoints);

closestPairs optimal(vector<Point> inputPoints);

closestPairs optimalRecursion(vector<Point> vectorOfPoints, vector<Point> ySorted){
    int size = vectorOfPoints.size();

    if (size <= 3) {
        return brute(vectorOfPoints);
    }

    int middlePosition = size / 2;
    closestPairs btwnHalvesResult;
    double middleX = (vectorOfPoints.begin() + middlePosition)->x;

    vector<Point> ySortedLeft, ySortedRight;
    for (Point p : ySorted) {
        (p.x <= middleX) ? ySortedLeft.push_back(p) : ySortedRight.push_back(p);
    }

    closestPairs lhs = optimalRecursion(vector<Point>(vectorOfPoints.begin(), vectorOfPoints.begin() + middlePosition), ySortedLeft);
    closestPairs rhs = optimalRecursion(vector<Point>(vectorOfPoints.begin() + middlePosition, vectorOfPoints.end()), ySortedRight);

    double minLeft = distanceFormula(lhs[0].first, lhs[0].second);
    double minRight = distanceFormula(rhs[0].first, rhs[0].second);
    double minBetweenHalves;

    if (abs(minLeft - minRight) < 1e-7) {
        btwnHalvesResult = lhs;
        btwnHalvesResult.insert(btwnHalvesResult.end(), rhs.begin(), rhs.end());
    } else {
        btwnHalvesResult = (minLeft < minRight) ? lhs : rhs;
    }

    minBetweenHalves = distanceFormula(btwnHalvesResult[0].first, btwnHalvesResult[0].second);

    // now compare minBetweenHalves to the shortest distance in the middle strip
    vector<Point> strip;
    for (Point p : ySorted) {
        if (abs(middleX - p.x) <= minBetweenHalves)
            strip.push_back(p);
    }

    // create pairs whose y-coords differ by at most minBetweenHalves
    vector<pair<Point, Point>> stripPairs;
        if (!(strip.empty())) {
            stripPairs = createStripPairs(strip, minBetweenHalves);
            sort(stripPairs.begin(), stripPairs.end(), sortPairsByDist);

            stripPairs = createStripPairs(strip, minBetweenHalves);
            sort(stripPairs.begin(), stripPairs.end(), sortPairsByDist);
        }

    return compareMin(btwnHalvesResult, stripPairs);
} // optimalHelper

int main(int argc, char *argv[]) {
    if (argc != 2 ) {
        cout <<"Usage: ./closestPair [brute | basic | optimal] " << endl;
        exit(1);
    }

    // correct number of arguments
    else {
        string approach = argv[1];

        if (!(approach == "brute") && !(approach == "basic") && !(approach == "optimal")) {
            cout <<"Usage: ./closestPair <brute><basic><optimal> " << std::endl;
            exit(1);
        }
        vector<Point> listOfPoints = get_input_remove_dup();

        if (listOfPoints.size() < 2) {
            cout << "Usage: Need at least 2 distinct points." << endl;
            exit(1);
        }
        else {
            closestPairs result;

            // brute, basic, optimal
            if (approach == "brute")
                result = brute(listOfPoints);

            else if (approach == "basic") {
                sort(listOfPoints.begin(), listOfPoints.end(), sortPoints);
                result = basic(listOfPoints);
            } else if (approach == "optimal") {
                sort(listOfPoints.begin(), listOfPoints.end(), sortPoints);
                result = optimal(listOfPoints);
            }

            //double min = distanceFormula((result)[0].first, (result)[0].second);

            //cout << fixed << "closest pair distance: " << min;

            printPairs(result);

            return 0;
        }

    }
} // main


closestPairs brute(vector<Point> inputPoints) {
    double min = DBL_MAX;
    closestPairs result;

    for (vector<Point>::iterator it1 = inputPoints.begin(); it1 != inputPoints.end() - 1; it1++) {
        for (vector<Point>::iterator it2 = it1 + 1; it2 != inputPoints.end(); it2++) {
            double dist = distanceFormula(*it1, *it2);

            if (abs(dist - min) < 1e-7){
                result.push_back(make_pair(*it1, *it2));
            }
            else if (dist < min) {
                min = dist;

                result.clear();
                result.push_back(make_pair(*it1, *it2));
            }
        }
    }
    return result;
} // brute

closestPairs basic(vector<Point> inputPoints) {
    int size = inputPoints.size();

    if (size <= 3) {
        return brute(inputPoints);
    }

    closestPairs btwnHalvesResult;
    int middlePosition = size / 2;


    closestPairs lhs = basic(vector<Point>(inputPoints.begin(), inputPoints.begin() + middlePosition));
    closestPairs rhs = basic(vector<Point>(inputPoints.begin() + middlePosition, inputPoints.end()));


    double minLeft = distanceFormula(lhs[0].first, lhs[0].second);
    double minRight = distanceFormula(rhs[0].first, rhs[0].second);
    double minBetweenHalves;


    if (abs(minLeft - minRight) < 1e-7 /*minLeft == minRight*/) {
        btwnHalvesResult = lhs;
        btwnHalvesResult.insert(btwnHalvesResult.end(), rhs.begin(), rhs.end());
    } else {
        btwnHalvesResult = (minLeft < minRight) ? lhs : rhs;
    }


    minBetweenHalves = distanceFormula(btwnHalvesResult[0].first, btwnHalvesResult[0].second);

    double middleX = (inputPoints.begin() + middlePosition)->x;
    vector<Point> strip;
    for (Point p : inputPoints) {
        if (abs(middleX - p.x) <= minBetweenHalves)
            strip.push_back(p);
    }


    sort(strip.begin(), strip.end(), sortByY);

    // create pairs whose y-coords differ by at most minBetweenHalves
    vector<pair<Point, Point>> stripPairs;
    if (!(strip.empty())) {
        stripPairs = createStripPairs(strip, minBetweenHalves);
        sort(stripPairs.begin(), stripPairs.end(), sortPairsByDist);

        stripPairs = createStripPairs(strip, minBetweenHalves);
        sort(stripPairs.begin(), stripPairs.end(), sortPairsByDist);
    }


    return compareMin(btwnHalvesResult, stripPairs);

} // basic


closestPairs optimal(vector<Point> inputPoints) {
    vector<Point> ySorted = inputPoints;
    sort(ySorted.begin(), ySorted.end(), sortByY);

    // sortPoints is sort by x by default
    return optimalRecursion(inputPoints, ySorted);
}

