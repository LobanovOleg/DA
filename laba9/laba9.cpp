#include <climits>
#include <iostream>
#include <vector>

using namespace std;
using ll = long long;

struct edge_t {
  int from, to;
  ll weigth;
  edge_t(int from, int to, ll weigth);
};

struct graph_t {
  vector<edge_t> edges;
  int vert;
  graph_t(int n);
};

edge_t::edge_t(int from, int to, ll weigth) {
  this->from = from;
  this->to = to;
  this->weigth = weigth;
}

graph_t::graph_t(int n) { this->vert = n + 1; }

void algorythm_Dijkstra(ll *distance, graph_t *graph) {
  for (int i = 0; i < graph->vert - 1; ++i) {
    ll dist[graph->vert];
    dist[i] = 0;
    for (int j = 0; j < graph->vert - 1; ++j) {
      if (j != i)
        dist[j] = LONG_MAX;
    }
    bool visited[graph->vert];
    for (int j = 0; j < graph->vert - 1; ++j) {
      visited[j] = false;
    }
    for (int j = 0; j < graph->vert - 1; ++j) {
      int from = -1;
      for (int k = 0; k < graph->vert - 1; ++k) {
        if (!visited[k] && (from == -1 || dist[k] < dist[from]))
          from = k;
      }
      if (dist[from] == LONG_MAX)
        break;
      visited[from] = true;
      for (auto &edge : graph->edges) {
        if (edge.from == from)
          dist[edge.to] = min(dist[edge.to], dist[edge.from] + edge.weigth);
      }
    }
    for (int j = 0; j < graph->vert - 1; ++j) {
      if (dist[j] == LONG_MAX)
        cout << "inf" << " ";
      else
        cout << dist[j] - distance[i] + distance[j] << " ";
    }
    cout << "\n";
  }
  cout << endl;
}

pair<ll *, bool> algorythm_Bellman_Ford(graph_t *graph) {
  auto *dist = new ll[graph->vert];
  for (int i = 0; i < graph->vert - 1; ++i)
    dist[i] = LONG_MAX;
  dist[graph->vert - 1] = 0;
  for (int i = 0; i < graph->vert - 1; ++i) {
    for (auto &edge : graph->edges) {
      if (dist[edge.from] < LONG_MAX)
        dist[edge.to] = min(dist[edge.to], dist[edge.from] + edge.weigth);
    }
  }
  for (auto &edge : graph->edges) {
    if (dist[edge.from] < LONG_MAX) {
      if (dist[edge.to] > dist[edge.from] + edge.weigth)
        return pair(nullptr, false);
    }
  }
  for (auto &edge : graph->edges)
    edge.weigth += dist[edge.from] - dist[edge.to];
  return pair(dist, true);
}

int main() {
  int n, m, i = 0;
  int a, b, weigth;
  cin >> n >> m;
  if (n > 2000 || n < 1 || m > 4000 || m < 1)
    return 0;
  auto *graph = new graph_t(n);
  for (; i < m; ++i) {
    cin >> a >> b >> weigth;
    graph->edges.emplace_back(a - 1, b - 1, weigth);
  }
  for (i = 0; i < n; ++i)
    graph->edges.emplace_back(n, i, 0);
  pair<ll *, bool> bellman_ford = algorythm_Bellman_Ford(graph);
  if (!bellman_ford.second)
    cout << "Negative cycle" << endl;
  else
    algorythm_Dijkstra(bellman_ford.first, graph);
  return 0;
}