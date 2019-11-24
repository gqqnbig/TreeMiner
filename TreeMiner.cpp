#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>

#include <string>
#include <iterator>
#include <sstream>

#define emptySet "[]"
#define joinOperator "join"

//#define debug
using namespace std;
typedef vector<int> vi;
const int N = 100000 + 10;
const int MinSup = 2;

string vectorToString(vector<int> &m, char *empty) {
	if (!m.empty()) {
		std::ostringstream oss;
		// Convert all but the last element to avoid a trailing ","
		copy(m.begin(), m.end() - 1, ostream_iterator<int>(oss, ","));

		// Now add the last element with no delimiter
		oss << m.back();
		return "[" + oss.str() + "]";
	} else
		return empty;
}

struct Pair {
  int content, position;
  Pair() {}
  Pair(int _x, int _p) {
	  content = _x;
	  position = _p;
  }

  string toString() {
	  return "[" + to_string(content) + "," + to_string(position) + "]";
  }

  string toLabelString() {
	  return "[" + string(1, 'A' - 1 + content) + "," + to_string(position) + "]";
  }

};
struct Scope {
  int tid; // tree id
  vi m;    // match label of the k-1 length prefix
  Pair s;  // scope of the last item
  Scope() {}
  Scope(int _tid, vi _m, int lx, Pair _s) {
	  tid = _tid;
	  m = _m;
	  m.push_back(lx);
	  s = _s;
  }

  std::string toString() {
	  return "(t=" + std::to_string(tid) + ", m=" + vectorToString(m, emptySet) + ", s=" + s.toString() + ")";
  }
};
struct Element {
  //Possible position
  Pair label;
  vector<Scope> scopes;

  //check support
  bool check() {
		int cnt = 0, checkedTree = -1;

		for (int i = 0; i < scopes.size(); i++) {
			int tid = scopes[i].tid;
			if (tid != checkedTree)
				cnt++, checkedTree = tid;
	  }
	  return cnt >= MinSup;
  }
} element[N];
struct Subtree {
  vi prefix;
  vector<Element> elements;

  string prefixToString() {
	  string s;
	  char buff[50];
	  for (int i = 0; i < prefix.size(); i++) {
		  if (prefix[i] == -1)
			  sprintf(buff, "%d ", prefix[i]);
		  else
			  printf(buff, "%c ", 'A' - 1 + prefix[i]);

		  s = s + buff;
	  }
	  return s;
  }

  //print the tree
  void output() {

	  if (elements.size() == 0)
		  return;
	  printf("prefix (class template): ");
	  if (prefix.size() == 0)
		  printf("empty");
	  else {
		  for (int i = 0; i < prefix.size(); i++)
			  if (prefix[i] == -1)
				  printf("%d ", prefix[i]);
			  else
				  printf("%c ", 'A' - 1 + prefix[i]);
	  }
	  puts("");
	  for (int i = 0; i < elements.size(); i++) {
		  // puts("----------");
		  printf("New node: ");

		  printf("%1$c %2$d (add node %1$c to DFS position %2$d)\n", 'A' + elements[i].label.content - 1, elements[i].label.position);
#ifdef debug
		  vector<Scope> &scopes = elements[i].scopes;
		  for (int j = 0; j < scopes.size(); j++) {
			  printf("%d,", scopes[j].tid);
			  for (int k = 0; k < scopes[j].m.size(); k++)
				  printf("%d%c", scopes[j].m[k],
						 k + 1 == scopes[j].m.size() ? ',' : ' ');
			  printf("[%d,%d]\n", scopes[j].s.content, scopes[j].s.position);
		  }
#endif
	  }
  }
};
void pt(vi v) {
	for (int i = 0; i < v.size(); i++)
		printf("%d ", v[i]);
	puts("");
}
void work(const vi &v, int tid) {
#ifdef debug
	for (int i = 0; i < v.size(); i++) {
		if (v[i] == -1)
			printf("%d ", v[i]);
		else
			printf("%c ", 'A' + v[i] - 1);
	}
	puts("\n~~~");
#endif
	int lx = 0;
	for (int i = 0; i < v.size(); i++) {
		if (v[i] == -1)
			continue;
		Scope scope;
		scope.tid = tid;
		scope.s.content = lx;
		for (int j = i + 1, tot = 1, ux = lx; j < v.size(); j++) {
			if (v[j] == -1)
				tot--;
			else
				tot++, ux++;
			if (tot == 0) {
				scope.s.position = ux;
				element[v[i]].label = Pair(v[i], -1);
				element[v[i]].scopes.push_back(scope);
				break;
			}
		}
		++lx;
	}
}
void Prefix_Generate(const vi &pre1, const Pair &label, vi &pre2) {
	pre2 = pre1;
	for (int i = 0, fa = 0; i < pre1.size(); i++) {
		if (pre1[i] == -1)
			continue;
		if (fa++ == label.position) {
			int cnt = 0;
			for (int j = i + 1; j < pre1.size(); j++) {
				if (pre1[j] != -1)
					cnt++;
				else
					cnt--;
			}
			for (int j = 0; j < cnt; j++)
				pre2.push_back(-1);
			break;
		}
	}
	pre2.push_back(label.content);
}
void TMdfs(Subtree P) {
	P.output();
	for (int i = 0; i < P.elements.size(); i++) {
		Subtree PP;
		Pair px = P.elements[i].label;
		Prefix_Generate(P.prefix, px, PP.prefix);
		vector<Scope> &scopex = P.elements[i].scopes;
		for (int j = 0; j < P.elements.size(); j++) {
			Pair py = P.elements[j].label;
			vector<Scope> &scopey = P.elements[j].scopes;
			// In-scope Test
			if (px.position == py.position) {
				Element eleIn;
				eleIn.label = Pair(py.content, py.position + 1); //(y,j+1)
				printf("In-scope join %s and %s, get %s:\n", px.toLabelString().c_str(), py.toLabelString().c_str(), eleIn.label.toLabelString().c_str());
				for (int r = 0; r < scopex.size(); r++)
					for (int t = 0; t < scopey.size(); t++) {
						//skip same element
						if ((&scopex[r]) == (&scopey[t]))
							continue;
						bool mtest = scopex[r].m == scopey[t].m;
						if (scopex[r].tid == scopey[t].tid && mtest &&
							scopex[r].s.content <= scopey[t].s.content &&
							scopex[r].s.position >= scopey[t].s.position) {

							Scope s = Scope(scopex[r].tid, scopex[r].m,
											scopex[r].s.content, scopey[t].s);
							printf("%s %s %s=%s.\n", scopex[r].toString().c_str(), joinOperator, scopey[t].toString().c_str(), s.toString().c_str());
							eleIn.scopes.push_back(s);
						}
					}
				if (eleIn.check()) {
					PP.elements.push_back(eleIn);
					puts("The pattern is frequent.");
				} else {
					puts("The pattern is not frequent.");
				}
			}
			// Out-scope Test
			// That position is -1 means the node is the root.
			if (py.position != -1) {
				Element eleOut;
				eleOut.label = Pair(py.content, py.position); //(y,j)
				for (int r = 0; r < scopex.size(); r++)
					for (int t = 0; t < scopey.size(); t++) {
						if (scopex[r].tid == scopey[t].tid && scopex[r].m == scopey[t].m &&
							scopex[r].s.position < scopey[t].s.content) {
							eleOut.scopes.push_back(Scope(scopex[r].tid, scopex[r].m,
														  scopex[r].s.content, scopey[t].s));
						}
					}
				if (eleOut.check())
					PP.elements.push_back(eleOut);
			}
		}
		TMdfs(PP);
	}
}
void F1() {
	int i, tot = 0, treeId = 1;
	vi v;
	while (~scanf("%d", &i)) {
		v.push_back(i);
		if (i == -1 && --tot == 0) {
			work(v, treeId++);
			v.clear();
		} else if (i != -1 && ++tot);
	}
	Subtree P1;
	for (int j = 1; j < N; j++) {
		if (element[j].check())
			P1.elements.push_back(element[j]);
	}
	TMdfs(P1);
}
int main() {
	// freopen("test.txt","r",stdin);
	freopen("../F5.data", "r", stdin);
	// freopen("../F5.data.out","w",stdout);
	F1();
	return 0;
}
