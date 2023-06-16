#include "def.hpp"
#include "graph.hpp"
#include "fibheap.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <random>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <set>
#include "timestamp.hpp"
#include "double.hpp"
#include "bc.hpp"
#include "file_format.hpp"

using std::greater;
using std::vector;
using std::pair;
using std::set;

const int MAX_DIR_LEN=64;
const int MAX_FILE_NAME_LEN=128;

char testDirName[MAX_DIR_LEN];

uint32_t pivotNo;

struct HLType {
  v_t ver;
  d_t dist;
  v_t pred;
  HLType() = default;
  HLType(v_t _ver,d_t _dist,v_t _pred):ver(_ver),dist(_dist),pred(_pred){}
};

KG<v_t,d_t> kg;

//std::vector<HLType>* hl;
vector<vector<HLType>> hl;

vector<pair<d_t,v_t> > M[G];
vector<pair<d_t,v_t> > treeM;
vector<bool> inTree;

std::vector<std::vector<v_t>> keySets;


bool cmp_HL(const HLType &a, const HLType &b) {
  return a.ver<b.ver;
}


d_t get_D(v_t u, v_t v) {
  size_t j = 0;
  d_t minDist = D_INF;
  for (size_t i = 0; i < hl[u].size(); ++i) {
    while (j < hl[v].size() && hl[v][j].ver < hl[u][i].ver) j += 1;
    if (j == hl[v].size()) break;
    if (hl[u][i].ver == hl[v][j].ver) {
      minDist=std::min(minDist,hl[u][i].dist+hl[v][j].dist);
    }    
} 
  return minDist;
}

d_t get_SP(v_t u, v_t v, vector<v_t> &sp) {
  size_t j = 0;
  d_t minDist = D_INF;
  v_t w=0;
  for (size_t i = 0; i < hl[u].size(); ++i) {
    while (j < hl[v].size() && hl[v][j].ver < hl[u][i].ver) j += 1;
    if (j == hl[v].size()) break;
    if (hl[u][i].ver == hl[v][j].ver && hl[u][i].dist+hl[v][j].dist<minDist) {
      minDist = hl[u][i].dist + hl[v][j].dist;
      w=hl[u][i].ver;
    }    
  }
  while (u != w) {
    sp.push_back(u);
    auto it = std::lower_bound(hl[u].begin(), hl[u].end(), HLType(w,0,0), cmp_HL);
    if (it->ver != w) {
      throw std::runtime_error("cannot get shortest path"); 
    }
    u=it->pred;
  }
  sp.push_back(w);
  vector<v_t> revVec;
  while (v != w) {
    revVec.push_back(v);
    auto it = std::lower_bound(hl[v].begin(), hl[v].end(), HLType(w,0,0), cmp_HL);
    if (it->ver != w) {
      throw std::runtime_error("cannot get shortest path"); 
    }
    v=it->pred;
  }
  if(!revVec.empty()){
    sp.push_back(revVec.back());
    revVec.pop_back();
  }
  return minDist;
}

d_t get_D_set(v_t u,v_t v,const vector<set<HLType,decltype(cmp_HL)*>> &setHLs){
  d_t minDist=D_INF;
  auto vIter=setHLs[v].begin();
  for(auto uIter=setHLs[u].begin();uIter!=setHLs[u].end();++uIter){
    while(vIter!=setHLs[v].end()&&vIter->ver<uIter->ver){
      ++vIter;
    }
    if(vIter==setHLs[v].end())break;
    if(uIter->ver==vIter->ver){
      minDist=std::min(minDist,uIter->dist+vIter->dist);
    }
  } 
  return minDist;
}

void get_static_HL() {
  // hl = new vector<HLType>[kg.size()];
  hl.resize(kg.size(),vector<HLType>());
  vector<long double> bc=NAMESPACE_BC::get_bc(kg, pivotNo);

  auto tmpArr = vector<std::pair<long double, v_t>>(kg.size());
  for (size_t i = 0; i < kg.size(); ++i) {
    tmpArr[i].first = bc[i];
    tmpArr[i].second = i;
  }
  std::sort(tmpArr.begin(), tmpArr.end(), greater<>());
  

  FibHeap<std::pair<d_t,v_t> >pq;
  vector<set<HLType,decltype(cmp_HL)*>> setHLs(kg.size(),set<HLType,decltype(cmp_HL)*> (cmp_HL));//temporily use sets to maintain HL 

  auto d = vector<d_t>(kg.size());
  auto pqIter = vector< FibHeap<std::pair<d_t, v_t>>::iterator>(kg.size());
  auto ok = vector<bool>(kg.size());
  auto prv= vector<v_t>(kg.size());
  
  for (v_t j = 0; j < kg.size(); ++j) {// init 
    d[j] = D_INF;
    pqIter[j] = nullptr;
    ok[j] = false;
  }
  for (size_t i = 0; i < kg.size(); ++i) {//enumerate entry in descending order of bc
    vector<v_t> verticeNeedUpdate;
    vector<v_t> modified;


    v_t src = tmpArr[i].second;//entry
    pq.clear();
    modified.push_back(src);
    d[src] = 0;
    pqIter[src] = pq.push(std::make_pair(d[src], src));
    prv[src]=src;

    for (v_t _ = 0; _ < kg.size(); ++_) {//dijkstra-like
      if(pq.empty())break;
      auto t = pq.top();
      pq.pop();
      ok[t.second]=true;
      if (double_le(d[t.second], get_D_set(t.second, src, setHLs))) {
        verticeNeedUpdate.push_back(t.second);
        for (auto e : kg.adjacent_edges(t.second)) {
          if (ok[e.to] == true) continue;
          if (pqIter[e.to] == nullptr) {//e.to is not in heap
            prv[e.to]=t.second;
            d[e.to]=d[t.second]+e.weight;
            pqIter[e.to]=pq.push(std::make_pair(d[e.to],e.to));
            modified.push_back(e.to);
          } else if (double_le(d[t.second] + e.weight, d[e.to])) {//e.to is in heap but its distance needs updating
            prv[e.to]=t.second;
            d[e.to]=d[t.second]+e.weight;
            pq.modify(pqIter[e.to],std::make_pair(d[e.to],e.to));
            modified.push_back(e.to);
          }
        }
      }
    } 
    for(v_t ver:verticeNeedUpdate){//update hl when 
      setHLs[ver].insert(HLType(src,d[ver],prv[ver]));
      // hl[ver].emplace_back(src,d[ver],prv[ver]);
      // fprintf(stderr,"src=%lu, bc of src=%.3f, new hl of ver %lu=",src,(double)bc[src],ver);
      // for(HLType sHL:setHLs[ver]){
      //   fprintf(stderr,"(%lu,%.3f) ",sHL.ver,(double)sHL.dist);
      // }
      // fprintf(stderr,"\n");
    }
    for(v_t ver:modified){
      d[ver]=D_INF;
      pqIter[ver]=nullptr;
      ok[ver]=false;
    }
  }
  uint64_t hlSum=0,hlMax=0;
  for(v_t ver=0;ver<kg.size();++ver){// sort hl of all vertices in order to getD and getSP
    hl[ver].reserve(setHLs[ver].size());
    for(HLType sHL:setHLs[ver]){
      hl[ver].push_back(sHL);
    }
    hlSum+=hl[ver].size();
    hlMax=max(hlMax,hl[ver].size());
    // sort(hl[ver].begin(),hl[ver].end(),cmp_HL);
  }
  fprintf(stderr,"hl: %.2f %lu\n",(double)hlSum/(double)kg.size(),hlMax);
}


d_t keyKG_plus(const vector<std::reference_wrapper<vector<v_t>>> &K,vector<v_t> &treeVertices,vector<std::pair<v_t,v_t> > &treeEdges) {

  if (K.empty()) return (d_t)0;
  
  d_t ans=D_INF;
  //construct M
  //M[i-1][j] first: min dist from group K[i] to hub[j], second: the vertex in K[i] that has the minimum dist to hub[j]
  //M[i-1][j]: the union of hub of all vertices in K[i]
  

  int64_t t1=micros();
  for (size_t i = 1; i < K.size(); ++i) {
    // int64_t t3=micros();
    // M[i-1].resize(kg.size(),std::make_pair(D_INF,-1));
    // int64_t t4=micros();
    // fprintf(stderr,"init: %ld\n",t4-t3);
    for (v_t v : K[i].get()) {
      for (HLType h : hl[v]) {
        if (h.dist < M[i - 1][h.ver].first) {
          M[i - 1][h.ver].first = h.dist;
          M[i-1][h.ver].second=v;
        }
      }  
    }
  }
  int64_t t2=micros();
  fprintf(stderr,"s1: %ld\n",t2-t1);

  t1=micros();
  vector<v_t> U;
  d_t minW = D_INF;
  for (v_t v0 : K[0].get()) {
    d_t sum=0;
    vector<v_t> u0;
    u0.reserve(K.size());
    u0.push_back(v0);
    for (size_t i = 1; i < K.size(); ++i) {//all other groups
      // for each group except K[0], get the vertex nearest to K[0]
      d_t minD = D_INF;
      v_t minVer=(v_t)-1;
      for (HLType h : hl[v0]) {
        if (h.dist + M[i - 1][h.ver].first < minD) {
          minD = h.dist + M[i - 1][h.ver].first;
          minVer=M[i-1][h.ver].second;
        }
      }
      sum += minD;
      u0.push_back(minVer);
    }
    if (sum < minW) {
      minW = sum;
      U=std::move(u0);
    }
  }
  t2=micros();
  fprintf(stderr,"s2: %lu\n",t2-t1);
  
  // auto *treeM = new std::pair<d_t, v_t>[kg.size()]();
  // bool *inTree = new bool [kg.size()]();
  
  t1=micros();
  // auto treeM=vector<std::pair<d_t,v_t>>(kg.size(),std::make_pair(D_INF,-1));
  // auto inTree=vector<bool>(K.size(),false);
  for (v_t u : U) {//enumerate the root of tree
    vector<v_t> treeMModified;// which indexes of treeM is modified
    for (HLType uHL : hl[u]) {//add u to tree, update treeM
      treeM[uHL.ver].first = uHL.dist;
      treeM[uHL.ver].second=u;
      treeMModified.push_back(uHL.ver);
    }

    for (v_t t : U) {//init
      inTree[t]=false;
    }
    inTree[u]=true;

    d_t tmpAns = 0;
    vector<v_t> tmpTreeVertices;
    vector<std::pair<v_t, v_t>> tmpTreeEdges;
    tmpTreeVertices.push_back(u);
    for (size_t treeSize = 1; treeSize < K.size(); ++treeSize) {
      v_t minS = (v_t)-1, minT = (v_t)-1;//S is in tree, T is out of tree
      d_t minDist = D_INF;
      for (v_t t : U) {
        if (inTree[t] == true) continue;
        //enumerate t outside tree
        for (HLType tHL : hl[t]) {
          d_t tmpDist = tHL.dist + treeM[tHL.ver].first;
          if (tmpDist < minDist) {
            minDist = tmpDist;
            minS = treeM[tHL.ver].second;
            minT = t;
          }
        }
      }
      if (minS == (v_t)-1 && minT == (v_t)-1) {//no vertices are out of tree
        break;
      }
      assert(minDist+eps<D_INF);
      vector<v_t> minS2minT;
      d_t distST = get_SP(minS, minT, minS2minT);
      tmpAns += distST;
      if(tmpAns>ans)break;
      for (size_t i = 0; i + 1 < minS2minT.size(); ++i) {
        tmpTreeEdges.emplace_back(minS2minT[i],minS2minT[i+1]);
        tmpTreeVertices.push_back(minS2minT[i+1]);
        for(HLType newNodeHL:hl[minS2minT[i+1]]){// update treeM
          if(newNodeHL.dist<treeM[newNodeHL.ver].first){
            treeM[newNodeHL.ver].first=newNodeHL.dist;
            treeM[newNodeHL.ver].second=minS2minT[i+1];
            treeMModified.push_back(newNodeHL.ver);
          }
        }
      }
    }
    if (tmpAns < ans) {
      ans = tmpAns;
      treeVertices=std::move(tmpTreeVertices);
      treeEdges=std::move(tmpTreeEdges); 
    }
    for(v_t modifiedVer:treeMModified){//clean treeM
      treeM[modifiedVer]=std::make_pair(D_INF,-1);
    }
  }
  t2=micros();
  fprintf(stderr,"s3: %lu\n",t2-t1);
  for(size_t i=1;i<K.size();++i){//clean
    for (v_t v : K[i].get()) {
      for (HLType h : hl[v]) {
        M[i-1][h.ver]=std::make_pair(D_INF,-1);
      }  
    }
  }
  return ans;
}

void discard_first_line(FILE *file) {
  while(true){
    int c=0;
    c=fgetc(file);
    if(c==EOF){
      fprintf(stderr,"Error: the file has only one line.\n");
      exit(-1);
    }
    if(c=='\n')break;
  }
}

void input_edges() {
  char srcFileName[MAX_FILE_NAME_LEN], fileName[MAX_FILE_NAME_LEN];
  sprintf(srcFileName,"%s/edges.txt",testDirName);
  sprintf(fileName,"%s/edges_formatted.txt",testDirName);
  format_file(srcFileName,fileName); 
  
  FILE* file=fopen(fileName,"r");
  if(file==nullptr){
    fprintf(stderr,"Error: cannot find file %s\n",fileName);
  }
  discard_first_line(file);
  std::random_device dev;
  std::string seedStr="fuck you nvidia";
  std::seed_seq mySeed(seedStr.begin(),seedStr.end());
  std::mt19937 mt(mySeed);
  std::uniform_real_distribution<double> realDistr(eps,1000.0);
  v_t u, v;
  while (true) {
    int rr=fscanf(file,"%lu%lu",&u,&v);
    if(rr==-1)break;
    kg.add_edge(u,v,realDistr(mt)); 
  }
  fclose(file);
}
void input_keymap() {
  char srcFileName[MAX_FILE_NAME_LEN], fileName[MAX_FILE_NAME_LEN];
  sprintf(srcFileName,"%s/keymap.txt",testDirName);
  sprintf(fileName,"%s/keymap_formatted.txt",testDirName);
  format_file(srcFileName,fileName); 
  FILE* file=fopen(fileName,"r");
  if(file==nullptr){
    fprintf(stderr,"Error: cannot find file %s\n",fileName);
  }
  discard_first_line(file);
  k_t key;
  v_t node;
  while (fscanf(file,"%lu%lu", &key, &node) != EOF) {
    if (keySets.size() < key + 1) {
      keySets.resize(key+1,std::vector<v_t>());
    }
    keySets[key].push_back(node);
  }
  for (auto vec : keySets) {
    vec.shrink_to_fit();
  }
  fclose(file);
}

int64_t output_keykg_ans(const vector<std::reference_wrapper<vector<v_t>>> &K){
  vector<v_t> treeVertices;
  vector<std::pair<v_t,v_t>> treeEdges;
  int64_t t1=micros();
  d_t ans = keyKG_plus(K, treeVertices, treeEdges);
  int64_t t2=micros();
  fprintf(stdout,"%f\n", ans);
  fprintf(stderr,"tree vertices:");
  for(v_t ver:treeVertices){
    fprintf(stderr,"%lu ",ver);
  }
  fprintf(stderr,"\n");
  fprintf(stderr,"kegkgpp time: %ld us\n",t2-t1);
  // treeVertices.clear();
  // treeEdges.clear();
  // d_t fake=keyKG_minus(K,treeVertices,treeEdges);
  // if(ans!=fake){
  //   fprintf(stderr,"plus=%.3f minus=%.3f WAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n",ans,fake);
  // }
  return t2-t1;
}
void answer_queries() {
  char srcFileName[MAX_FILE_NAME_LEN], fileName[MAX_FILE_NAME_LEN];
  sprintf(srcFileName,"%s/queries.txt",testDirName);
  sprintf(fileName,"%s/queries_formatted.txt",testDirName);
  format_file(srcFileName,fileName); 
  FILE* file=fopen(fileName,"r");
  if(file==nullptr){
    fprintf(stderr,"Error: cannot find file %s\n",fileName);
  }
  discard_first_line(file);
  int q, prv = -1,keySetNo;
  int64_t totalTime[G];
  int queryCount[G];
  memset(totalTime,0,sizeof(totalTime));
  memset(queryCount,0,sizeof(queryCount));
  for(auto & i : M){// init M[][]
    i.resize(kg.size(),std::make_pair(D_INF,-1));
  }
  treeM.resize(kg.size(),std::make_pair(D_INF,-1));//init treeM
  inTree.resize(kg.size(),false);
  vector<std::reference_wrapper<vector<v_t>>> K;
  while (fscanf(file,"%d%d", &q, &keySetNo) != EOF) {
    if (prv != -1 && q != prv) {
      totalTime[K.size()]+=output_keykg_ans(K);
      queryCount[K.size()]+=1;
      K.clear();
    }
    K.emplace_back(keySets[(size_t)keySetNo]);
    prv=q;
  }
  totalTime[K.size()]+=output_keykg_ans(K);
  queryCount[K.size()]+=1;
  for(int i=1;i<G;++i){
    fprintf(stderr,"g = %d, %d queries, total time = %lu us, mean time = %.1f us\n",i,queryCount[i],totalTime[i],(double)totalTime[i]/(double)queryCount[i]);
  }
  fclose(file);
}

int main(int argc,char **argv) {
  if (argc != 3) {
    printf("Too many or too few args.\n");
    return -1;
  }
  if (strlen(argv[1]) >= MAX_DIR_LEN) {
    printf("Arg Error: the path to data directory is too long.\n");
    return -1;
  }
  char* endPtr=nullptr;
  uint64_t argv2=strtoul(argv[2],&endPtr,10);
  if(argv2>1000){
    fprintf(stderr,"Arg Error: Too many pivots.\n");
    return -1;
  }
  else if(argv2==0){
    fprintf(stderr,"Arg Error: pivot number cannot be 0.\n");
  }
  pivotNo=(uint32_t)argv2;
  
  fprintf(stderr,"Program starts: data directory = %s, pivot number = %u\n",argv[1],pivotNo);

  strcpy(testDirName, argv[1]);
  input_edges();
  input_keymap();
  fprintf(stderr,"static HL start\n");
  int64_t t1=micros();
  get_static_HL();
  int64_t t2=micros();
  fprintf(stderr,"get static HL: %ld us\n",t2-t1);
  fprintf(stderr,"static HL end\n");
  answer_queries();
  return 0;
}