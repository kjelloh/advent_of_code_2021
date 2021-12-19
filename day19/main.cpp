#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <algorithm>
#include <tuple>
#include <array>

extern char const* pTest;
extern char const* pData;

using Result = size_t;
using Answers = std::vector<std::pair<std::string,Result>>;

struct Vector {
  std::vector<int> v{0,0,0};
  Vector(int x=0,int y=0, int z=0) : v{x,y,z} {}
  int x() {return v[0];}
  int y() {return v[1];}
  int z() {return v[2];}
  Vector operator-(Vector const& other) {
    Vector result{};
    std::transform(v.begin(),v.end(),other.v.begin(),std::back_inserter(result.v),[](int c1,int c2) {
      return c1-c2;
    });
    return result;
  }
  bool operator==(Vector const& other) const {
    return (v==other.v);
  }
};
using Vectors = std::vector<Vector>;
struct Scanner {
  int id{};
  Vectors beacons{};
};
using Scanners = std::vector<Scanner>;
using Model = Scanners;
using Beacons = Vectors;

/*
In total, each scanner could be in any of 24 different orientations: 
facing positive or negative x, y, or z, 
and considering any of four directions "up" from that facing.
*/

std::pair<std::string,std::string> split(std::string const& line,std::string delim) {
  if (auto pos = line.find(delim); pos != std::string::npos) {
    auto left = line.substr(0,pos);
    auto right = line.substr(pos + delim.size()); 
    return {left,right};
  }
  else {
    return {line,""};
  }
}

std::tuple<int,int,int> split_ints(std::string const& line) {
  auto [xs,right] = split(line,",");
  auto [ys,zs] = split(right,",");
  std::cout << "\n{" << xs << "," << ys << "," << zs << "}";
  return {std::stoi(xs),std::stoi(ys),std::stoi((zs==""?"0":zs))};
}

bool contains(std::string const& token,std::string const& key) {
  return (token.find(key) != std::string::npos);
}

Model parse(auto& in) {
    Model result{};
    std::string line{};
    int state{0};
    Scanner scanner{};
    while (std::getline(in,line)) {
      if (line.size()>0) {
        if (contains(line,"scanner")) {
          auto [left,right] = split(line,"scanner ");
          auto [sId,tail] = split(right," ");
          try {
            auto id = std::stoi(sId);
            scanner = Scanner{id,{}};
          }
          catch (std::exception const& e) {
            std::cout << "\nERROR - parse scanner id failed. Excpetion=" << e.what();
          }
        }
        else {
          try {
            auto [x,y,z] = split_ints(line);
            scanner.beacons.push_back({x,y,z});
          }
          catch (std::exception const& e) {
            std::cout << "\nERROR - parse ints falied. Excpetion=:" << e.what();
          }
        }
      }
      else if (scanner.beacons.size()>0) {
        result.push_back(scanner);
        scanner = {};    
      }
    }
    if (scanner.beacons.size()>0) result.push_back(scanner);
    return result;
}

std::vector<Scanner> scanner_permutations(Scanner const& scanner) {
  std::vector<Scanner> result;
  std::vector<Vector> faces{{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}};
  for (Vector const& face : faces) {
    // for facing {1,0,0} we can choose up as {0,1,0}, {0,-1,0}, {0,0,1} or {0,0,-1}
    // for facing {-1,0,0} we can choose the same up options
    // for facing {0,1,0} we can choose up as {1,0,0}, {-1,0,0}, {0,0,1}, {0,0,-1}

  };
  return result;
}

using RelativeEdge = Vector;
using RelativeEdges = std::vector<RelativeEdge>;
// Basically a graph of edges between viewed beacons
class Shape {
public:
  Shape(Scanner const& scanner) : vertecies{scanner.beacons}{
    for (int i=0;i<vertecies.size();i++) {
      for (int j=0;j<vertecies.size();j++) {
        edges.push_back(vertecies[i] - vertecies[j]);
      }
    }
  }
  Vectors vertecies;
  RelativeEdges edges;
};

// The View defines a rotation that defines how we view a shape
struct View {
  Shape shape(Shape const& shape) const {
    std::string caption{"\nViewer::shape:"};
    // transform shape as defined by this view
    std::cout << caption << " TODO: Transform provided shape as defined by this view";
    return shape;
  }
};

/*
For a cartesian coordinate system the rotation matrices are

Rx90 =  1  0  0   Rotate 90 degrees counter clockwise around x-axis
        0  0 -1
        0  1  0

Ry90 =  0  0  1
        0  1  0
       -1  0  0

Rz90 =  0 -1  0
        1  0  0
        0  0  1
*/

// See 3d_rotations_matrices.png (https://github.com/kjelloh/advent_of_code_2021/tree/main/day19 )
std::array<std::array<int,3>,3> Rx90 = {{
   {1,0,0}
  ,{0,0,-1}
  ,{0,1,0}}};
std::array<std::array<int,3>,3> Ry90 = {{
   {0,0,1}
  ,{0,1,0}
  ,{-1,0,0}}};
std::array<std::array<int,3>,3> Rz90 = {{
   {0,-1,0}
  ,{1,0,0}
  ,{0,0,-1}}};


class Viewer {
public:
  std::vector<View> views;
  Viewer() {
    std::string caption{"\nViewer():"};
    // Create all 24 views
    std::vector<Vector> faces{{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}};
    for (Vector const& face : faces) {
      
    };
    std::cout << caption << " TODO: generate all 24 permutations to view a shape";
  }
};

int match_count(Shape const& shape_1,Shape const& shape_2) {
  std::string caption{"\nmatch_count:"};

  // return the number of matching edges between provided shapes
  int result{0};
  for (auto const& edge1 : shape_1.edges) {
    for (auto const& edge2 : shape_2.edges) {
      if (edge1 == edge2) ++result;
    }
  }
  return result;
}

Beacons find_beacons(Scanners const& scanners) {
  std::string caption{"\nfind_beacons:"};
  Beacons result;
  // Given two scanners are in the same coorodinate system,
  // Then -> Two scanners see the same beacons if > 12 of the "edges" between beacons are the same
  Shape shape_0{scanners[0]};
  Shape shape_1{scanners[1]};
  std::copy(shape_0.vertecies.begin(),shape_0.vertecies.end(),std::back_inserter(result));
  Viewer viewer{}; // 24 ways to view beacons seen by a scanner
  for (auto const& view : viewer.views) {
    auto viewed_shape_1 = view.shape(shape_1);
    if (match_count(shape_0,viewed_shape_1)>12) {
      // The view defines now to orient shape_1 to the same system as shape_0
      std::copy(shape_1.vertecies.begin(),shape_0.vertecies.end(),std::back_inserter(result));
    }
  }
  std::cout << caption << " TODO: Compare all scanner shapes";
  return result;
}

namespace part1 {
  Result solve_for(char const* pData) {
    std::string caption{"\nsolve_for:"};
    Result result{};
    std::stringstream in{ pData };
    auto scanners = parse(in);
    std::cout << caption << " scanner count " << scanners.size();
    auto beacons = find_beacons(scanners);
    result = beacons.size();
    return result;
  }
}

namespace part2 {
  Result solve_for(char const* pData) {
      Result result{};
      std::stringstream in{ pData };
      auto data_model = parse(in);
      return result;
  }
}

int main(int argc, char *argv[])
{
  Answers answers{};
  answers.push_back({"Part 1 Test",part1::solve_for(pTest)});
  answers.push_back({"Part 1     ",part1::solve_for(pData)});
  // answers.push_back({"Part 2 Test",part2::solve_for(pTest)});
  // answers.push_back({"Part 2     ",part2::solve_for(pData)});
  for (auto const& answer : answers) {
    std::cout << "\nanswer[" << answer.first << "] " << answer.second;
  }
  // std::cout << "\nPress <enter>...";
  // std::cin.get();
  std::cout << "\n";
  return 0;
}

char const* pTest = R"(--- scanner 0 ---
0,2
4,1
3,3

--- scanner 1 ---
-1,-1
-5,0
-2,1)";
char const* pData = R"(--- scanner 0 ---
579,799,416
48,82,-27
474,-502,534
652,-594,511
-609,574,-469
352,-544,-666
-780,740,353
-33,-102,-83
384,-640,-624
470,-559,544
468,864,533
670,759,-677
-700,873,335
-747,-486,-412
349,-584,-782
-382,-890,249
-567,658,-522
-689,659,-426
531,820,-584
-738,-569,-457
-369,-772,414
-816,800,310
601,763,-473
355,790,421
-629,-538,-431
-387,-903,381

--- scanner 1 ---
961,715,-541
-390,653,-337
475,-505,-519
-398,393,554
-543,-651,892
917,-691,745
-333,512,638
-447,-609,924
949,600,-504
-487,-493,927
959,652,536
860,-802,675
425,-583,-486
31,89,-38
856,-598,606
-729,-629,-505
-381,635,-466
-728,-658,-697
940,447,580
423,-556,-430
921,566,529
-385,763,-451
82,-46,86
863,628,-559
-792,-638,-728
-441,535,654

--- scanner 2 ---
-619,-740,-444
284,732,-285
349,769,-397
-800,371,856
-154,-142,35
-588,-753,-593
433,-771,646
426,648,604
-469,-701,964
-436,-703,873
-831,445,878
-68,-35,148
433,-910,547
321,677,-474
658,-831,-735
-708,471,-612
532,743,654
-599,-758,926
543,-753,-648
445,-815,-781
-682,-685,-597
491,-773,545
-814,389,-621
-712,458,817
-726,309,-547
422,838,592

--- scanner 3 ---
-742,640,493
468,610,-347
-656,680,388
533,814,631
-501,744,-596
472,-555,-383
-55,12,40
603,-399,668
-422,891,-618
-505,-699,597
-520,646,478
-523,-607,-421
-487,-491,600
479,-501,677
475,661,-559
428,770,510
-526,-596,-325
443,-605,-422
374,-553,-286
548,-425,783
-570,841,-566
-401,-666,575
605,756,519
469,566,-488
-494,-573,-565

--- scanner 4 ---
-24,-9,-13
693,825,-612
-659,674,730
726,696,493
-602,-470,716
-548,804,-616
-273,-440,-429
755,920,-722
591,-810,463
104,132,31
-798,656,824
701,-689,-711
-566,-637,674
800,-828,-711
901,636,510
-548,-422,677
592,-714,585
-775,678,794
-336,-418,-360
906,716,416
-738,825,-594
-360,-472,-355
-647,851,-605
802,-691,-653
743,730,-718
676,-767,543

--- scanner 5 ---
295,-829,-370
-646,-497,-795
785,-566,682
331,-823,-362
396,624,-653
-345,390,-837
-854,-521,625
-776,-569,671
-719,302,694
775,-608,710
618,434,807
613,-547,645
-691,-468,-758
-857,329,748
366,768,-662
-46,-127,-7
-368,546,-822
-855,-613,723
477,-938,-370
727,347,755
803,431,840
-390,421,-837
-915,322,743
-713,-449,-683
322,664,-548

--- scanner 6 ---
-478,-795,-399
636,601,800
475,790,-480
399,-509,961
48,51,76
-372,687,518
429,-773,-421
-854,749,-409
-751,809,-318
379,-786,-305
-764,-417,427
-712,-535,446
552,-546,960
-809,832,-531
653,710,892
483,781,-545
-104,95,187
378,-509,931
-505,589,580
-681,-426,472
499,783,-725
-347,-777,-366
452,-886,-397
-360,-782,-234
720,662,828
-418,474,502

--- scanner 7 ---
-747,-527,-295
-383,523,528
722,-666,942
622,-640,-526
-722,-449,-374
822,-614,982
531,486,601
-473,-615,807
658,-592,-519
546,512,-555
801,-857,967
-487,406,-646
-20,-36,160
-491,411,-601
596,468,439
550,606,-623
-705,-445,-317
636,-516,-380
699,575,-610
-472,-687,821
522,380,519
-563,527,518
-336,449,-620
-74,-172,0
-502,-611,738
-473,453,641

--- scanner 8 ---
-325,277,621
-346,432,631
648,-738,463
-284,-886,-509
610,-583,-878
429,537,390
-366,-908,-467
661,-654,541
567,-557,-727
605,-763,488
-379,-707,832
583,609,339
823,713,-542
888,705,-461
-262,393,-504
458,674,373
-329,-650,646
572,-602,-723
-237,503,-467
-288,-829,-492
-5,6,-132
-386,307,509
-383,-599,733
99,-170,-55
-249,384,-440
830,694,-667

--- scanner 9 ---
597,-475,-339
821,675,845
-364,-312,543
414,576,-313
743,721,797
-328,-320,537
-749,800,-693
386,653,-405
-661,726,-773
-95,72,62
-643,791,-611
-647,-638,-624
822,773,753
-735,-677,-711
613,-745,869
658,-708,898
487,-413,-244
491,-425,-434
-646,695,819
349,464,-350
-510,-320,428
-818,732,800
680,-633,917
-766,-699,-637
74,149,-8
-808,760,758

--- scanner 10 ---
671,-555,-655
672,548,433
-684,363,-477
628,600,354
720,502,355
792,-806,496
397,393,-396
-438,403,762
-573,-435,-449
-475,291,827
397,400,-473
434,477,-433
-559,-598,685
-464,-648,579
-625,390,-456
657,-513,-539
773,-881,518
-653,-440,-543
744,-918,563
-672,386,-603
-147,-172,17
647,-507,-645
-369,-586,689
-530,480,790
-593,-615,-526
-35,-50,-86

--- scanner 11 ---
-708,-978,-663
715,-781,-742
680,759,-622
-262,303,742
819,750,-718
-406,333,733
881,-642,685
-701,-920,-668
654,-831,-823
557,760,713
565,725,753
940,-709,735
-683,664,-675
933,-652,865
162,-1,80
-484,-431,902
-599,740,-760
-596,555,-727
763,-827,-746
-366,259,628
-609,-453,917
-728,-782,-614
-640,-408,917
463,792,860
30,-171,-39
742,714,-656

--- scanner 12 ---
-557,-594,-751
535,-288,-328
522,-780,851
577,462,-599
-378,644,873
506,-435,-371
562,-830,712
490,514,-708
-404,624,-594
620,-862,805
30,-32,-49
-485,-641,-711
-470,546,874
445,575,738
442,-439,-322
-423,587,-766
-722,-561,777
-794,-556,932
472,521,876
-426,598,795
-666,-473,931
465,526,934
-41,29,133
-673,-676,-778
-465,559,-756
574,429,-784

--- scanner 13 ---
-512,-780,-383
-634,-328,271
850,517,723
409,-606,298
-639,-724,-396
380,-651,335
100,-55,35
-752,362,545
735,371,731
739,589,-529
-411,665,-757
656,-371,-623
827,524,-467
708,-458,-468
833,537,-431
-488,714,-796
-610,-752,-415
749,-364,-620
-509,-350,256
-478,-305,256
-856,406,431
361,-673,410
741,335,723
-436,839,-789
22,58,-107
-712,436,440

--- scanner 14 ---
-460,675,-680
431,-518,-720
410,-453,-630
546,395,-372
-809,-922,-637
-476,-447,567
-385,609,-563
-679,-919,-628
496,378,-277
481,-504,704
-668,-866,-537
401,-490,-849
-432,672,-630
606,-464,582
711,311,823
-589,487,725
848,340,840
2,45,75
-564,530,750
559,334,-368
783,465,874
-533,-497,381
-480,543,672
-425,-501,480
451,-443,592
95,-118,129

--- scanner 15 ---
901,-715,774
-447,800,-713
-499,-598,656
-530,755,-704
613,743,-575
-57,-132,9
863,-629,781
859,628,565
383,-740,-702
563,-720,-625
698,786,-655
388,-660,-608
802,739,635
-499,781,-595
-391,-575,672
-415,-629,-674
-438,-641,-548
697,690,-533
-545,430,383
-442,-688,-695
882,-510,730
-505,546,386
-456,-456,756
798,535,704
100,-5,81
-601,588,418

--- scanner 16 ---
645,-660,-815
686,-400,877
-788,541,-551
-582,-777,-582
549,-417,906
-726,-490,687
0,107,10
-169,-72,58
621,-366,752
-751,-515,730
800,-659,-703
-545,700,768
725,515,512
-482,-755,-724
-571,851,725
-816,457,-490
737,678,-369
557,694,-371
-648,-739,-637
717,434,447
-642,-568,674
-880,524,-442
585,-617,-730
706,640,505
492,702,-362
-590,696,763

--- scanner 17 ---
-786,419,-780
580,542,522
-753,-461,678
-671,506,-702
660,-476,-725
-723,-515,665
491,710,-689
-659,-635,-482
460,561,-702
4,11,-37
-133,118,35
-477,757,820
-550,-652,-390
-828,-586,678
-449,680,728
554,557,-634
598,607,360
539,-552,439
686,533,440
-412,869,783
617,-325,-699
543,-372,449
613,-462,562
-598,-719,-454
-573,373,-764
655,-480,-680

--- scanner 18 ---
-626,741,779
-621,839,736
870,-429,299
-329,-456,-530
-481,-415,-436
474,-784,-723
769,358,-552
-484,-508,-443
862,-314,445
72,37,-49
707,453,-662
-564,805,649
862,-423,263
-328,-337,452
757,548,-556
437,-755,-678
544,738,582
-292,-331,465
483,751,701
-633,427,-669
-390,-425,517
505,825,670
-724,453,-680
-534,474,-658
549,-803,-745

--- scanner 19 ---
885,-364,-767
-758,-757,-795
-620,501,549
-808,378,-727
-652,-436,692
-654,530,693
563,-461,767
-36,-26,43
-602,-465,731
-643,533,789
754,-346,-734
81,100,-56
697,508,518
614,-468,555
541,493,-604
-818,-793,-838
484,497,-614
872,-359,-899
-882,491,-615
-891,-677,-813
519,-462,626
-817,602,-732
844,429,496
576,675,-637
-600,-367,828
794,577,548

--- scanner 20 ---
521,799,641
-814,-478,803
564,734,-498
-816,-536,666
-505,755,-478
416,854,546
-715,-478,664
626,864,-562
446,818,519
346,-834,447
322,-829,393
633,-584,-876
-483,786,-552
-488,391,596
724,-595,-850
-397,424,594
534,-573,-829
621,821,-464
-544,667,-565
-757,-471,-755
-47,63,-74
259,-760,415
-707,-577,-850
-435,554,651
-769,-467,-945

--- scanner 21 ---
693,-762,-734
616,828,628
-684,-626,636
-595,479,-482
-383,525,-446
674,-607,-772
-568,-625,-730
-392,572,557
782,844,715
682,866,573
-445,-535,-656
-363,646,506
448,-368,481
56,54,26
-671,-572,606
-72,160,-43
513,604,-739
538,579,-696
476,-452,542
-684,-501,711
384,-435,433
-455,-715,-660
-471,488,-412
622,657,-683
-315,512,408
719,-611,-777

--- scanner 22 ---
-426,-460,-817
430,-548,757
17,-21,65
645,-446,-810
-502,-376,-732
702,-493,-691
312,721,-517
477,-544,865
283,-543,859
-616,518,-622
676,-492,-734
332,738,-554
-96,88,-105
-863,-797,685
-934,800,393
-454,579,-687
498,874,776
-752,909,393
-611,-463,-784
473,807,749
535,829,715
-898,794,385
369,646,-663
-444,537,-659
-907,-693,571
-833,-653,676

--- scanner 23 ---
614,503,499
-741,-573,-430
666,496,-326
586,590,-331
-874,485,504
70,134,-24
-764,665,-640
709,-743,589
-855,603,556
-860,543,-641
-739,-631,-531
680,-638,-489
-799,-406,850
870,-642,-477
-67,-30,81
801,-715,512
-878,-631,-432
-783,-619,896
764,-780,400
567,650,495
-829,494,587
555,524,472
-817,623,-556
646,573,-506
775,-532,-539
-721,-521,763

--- scanner 24 ---
-804,350,366
456,-494,-642
363,-619,389
-411,-594,552
261,606,745
284,599,-445
341,-681,423
372,534,-507
278,586,-409
-878,381,388
-1,-117,40
-791,469,427
-127,5,-73
-670,-495,-397
435,-446,-785
543,-509,-828
-403,-518,594
-734,-428,-464
296,-644,362
-858,402,-681
376,752,753
-849,-457,-445
-892,315,-738
370,788,745
-794,272,-743
-418,-406,604

--- scanner 25 ---
86,149,-150
-572,-670,-518
748,-704,-522
569,-536,361
-723,-522,310
563,566,-778
-419,589,553
-665,-309,333
-597,-464,-470
829,-696,-457
739,901,432
806,837,480
561,-471,352
-628,-622,-575
722,809,541
-623,602,-609
585,-716,359
623,-685,-515
-470,657,-672
-474,589,-578
412,592,-735
-286,527,455
1,56,27
567,611,-773
-332,511,668
-768,-400,393)";
