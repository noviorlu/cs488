// Termm--Fall 2024

#include <algorithm>
#include <cstdio>

#include <iostream>

#include "maze.hpp"

Maze::Maze( size_t D )
	: m_dim( D )
{
	m_values = new int[ D * D ];

	reset();
}

void Maze::reset()
{
	size_t sz = m_dim*m_dim;
	std::fill( m_values, m_values + sz, 0 );
}

Maze::~Maze()
{
	delete [] m_values;
}

size_t Maze::getDim() const
{
	return m_dim;
}

int Maze::getValue( int x, int y ) const
{
	return m_values[ y * m_dim + x ];
}

void Maze::setValue( int x, int y, int h )
{
	m_values[ y * m_dim + x ] = h;
}

int perm[24][4] = {
	0,1,2,3,
	0,1,3,2,
	0,2,1,3,
	0,2,3,1,
	0,3,1,2,
	0,3,2,1,

	1,0,2,3,
	1,0,3,2,
	1,2,0,3,
	1,2,3,0,
	1,3,0,2,
	1,3,2,0,

	2,1,0,3,
	2,1,3,0,
	2,0,1,3,
	2,0,3,1,
	2,3,1,0,
	2,3,0,1,

	3,1,2,0,
	3,1,0,2,
	3,2,1,0,
	3,2,0,1,
	3,0,1,2,
	3,0,2,1,
};

int Maze::numNeighbors(int r, int c) {
	return getValue(r-1,c) + getValue(r+1,c) + getValue(r,c-1) + getValue(r,c+1);
}

// Print ASCII version of maze for debugging
void Maze::printMaze() {
	int i,j;
	for (i=0; i<m_dim; i++) {
		for (j=0; j<m_dim; j++) { 
			if ( getValue(i,j)==1 ) {
				printf("X");
			} else {
				printf(" ");
			}
		}
		printf("\n");
	}
}

std::vector<glm::vec3> Maze::getVertices(){
	// sort by the second element of the unordered map
	std::vector<glm::vec3> vertices;
	
	std::vector<std::pair<glm::vec3, int>> sortedVertices(m_Vertices.begin(), m_Vertices.end());
	std::sort(sortedVertices.begin(), sortedVertices.end(), [](const std::pair<glm::vec3, int>& a, const std::pair<glm::vec3, int>& b) {
		return a.second < b.second;
	});

	for (const auto& vertex : sortedVertices) {
		vertices.push_back(vertex.first);
	}

	return vertices;
}

void Maze::generateGeometry(){
	// concat m_values with 0s on the border
	int** m_values_ext = new int*[m_dim+2];
	for (int i = 0; i < m_dim + 2; ++i) {
		m_values_ext[i] = new int[m_dim + 2];
		for (int j = 0; j < m_dim + 2; ++j) {
			if (i == 0 || j == 0 || i == m_dim + 1 || j == m_dim + 1) {
				m_values_ext[i][j] = 0;
			} else {
				m_values_ext[i][j] = m_values[(i - 1) * m_dim + (j - 1)];
			}
		}
	}

	int i, j;
	for (i=1; i <= m_dim+1; i++) {
		// for row walls
		for (j=1; j <= m_dim+1; ) {
			int row = j;
			while (j <= m_dim && (m_values_ext[i][j] ^ m_values_ext[i-1][j]) == 1) {
				j++;
			}

			if(j - row > 0){
				// two points for the edge then extend to wall
				int x = i - 1;
				int z1 = row - 1;
				int z2 = j - 1;
				int idx = m_Vertices.size();
				auto v1 = glm::vec3(x, 0, z1);
				auto v2 = glm::vec3(x, 0, z2);
				auto v3 = glm::vec3(x, 1, z1);
				auto v4 = glm::vec3(x, 1, z2);

				if(m_Vertices.find(v1) == m_Vertices.end()) m_Vertices[v1] = idx++;
				if(m_Vertices.find(v2) == m_Vertices.end()) m_Vertices[v2] = idx++;
				if(m_Vertices.find(v3) == m_Vertices.end()) m_Vertices[v3] = idx++;
				if(m_Vertices.find(v4) == m_Vertices.end()) m_Vertices[v4] = idx++;

				m_triangles.push_back(glm::uvec3(m_Vertices[v1], m_Vertices[v2], m_Vertices[v3]));
				m_triangles.push_back(glm::uvec3(m_Vertices[v2], m_Vertices[v4], m_Vertices[v3]));
			}
			else j++;
		}
		// for column walls
		for(j=1; j <= m_dim+1; ){
			int col = j;
			while(j <= m_dim && (m_values_ext[j][i] ^ m_values_ext[j][i-1]) == 1){
				j++;
			}

			if(j - col > 0){
				// two points for the edge then extend to wall
				int x1 = col - 1;
				int x2 = j - 1;
				int z = i - 1;
				int idx = m_Vertices.size();
				auto v1 = glm::vec3(x1, 0, z);
				auto v2 = glm::vec3(x2, 0, z);
				auto v3 = glm::vec3(x1, 1, z);
				auto v4 = glm::vec3(x2, 1, z);

				if(m_Vertices.find(v1) == m_Vertices.end()) m_Vertices[v1] = idx++;
				if(m_Vertices.find(v2) == m_Vertices.end()) m_Vertices[v2] = idx++;
				if(m_Vertices.find(v3) == m_Vertices.end()) m_Vertices[v3] = idx++;
				if(m_Vertices.find(v4) == m_Vertices.end()) m_Vertices[v4] = idx++;

				m_triangles.push_back(glm::uvec3(m_Vertices[v1], m_Vertices[v2], m_Vertices[v3]));
				m_triangles.push_back(glm::uvec3(m_Vertices[v2], m_Vertices[v4], m_Vertices[v3]));
			}
			else j++;
		}
	}

	// free m_values_ext
	for (int i = 0; i < m_dim + 2; ++i) {
		delete[] m_values_ext[i];
	}
	delete[] m_values_ext;
}

void Maze::recDigMaze(int r, int c) {
	int* p;
	p = perm[random()%24];
	for (int i=0; i<4; i++) {
		switch (p[i]) {
		case 0:
			if ( r > 1 && getValue(r-1,c) && numNeighbors(r-1,c)==3 ) {
				setValue(r-1,c,0);
				recDigMaze(r-1,c);
			}
			break;
		case 1:
			if ( r < m_dim-2 && getValue(r+1,c) && numNeighbors(r+1,c)==3 ) {
				setValue(r+1,c,0);
				recDigMaze(r+1,c);
			}
			break;
		case 2:
			if ( c > 1 && getValue(r,c-1) && numNeighbors(r,c-1)==3 ) {
				setValue(r,c-1,0);
				recDigMaze(r,c-1);
			}
			break;
		case 3:
			if ( c < m_dim-2 && getValue(r,c+1) && numNeighbors(r,c+1)==3 ) {
				setValue(r,c+1,0);
				recDigMaze(r,c+1);
			}
			break;
		}
	} 
}

void Maze::digMaze()
{
	int i,j;
	// set all values to 1
	for (i=0;i<m_dim;i++) {
		for (j=0;j<m_dim;j++) {
			setValue(i,j,1);
		}
	}

	// clear out rings
	for (i=1; i<m_dim; i+=2) {
		for (j=i; j<m_dim-i; j++) {
			setValue(i,j,0);
			setValue(m_dim-i-1,m_dim-j-1,0);
			setValue(j,i,0);
			setValue(m_dim-j-1,m_dim-i-1,0);
		}
	}
	// Open and Block rings
	int s,pm;
	s=random()%(11-4+1)+4;
	pm = random()%2;
	setValue(s,2,0);
	if (pm)  { setValue(s+1,1,1); } else { setValue(s-1,1,1); }

	s=random()%(11-4+1)+4;
	setValue(s,13,0);
	if (pm)  { setValue(s-1,14,1); } else { setValue(s+1,14,1); }


	pm = random()%2;
	s=random()%(10-5+1)+5;
	setValue(4,s,0);
	if (pm) { setValue(3,s+1,1); } else { setValue(3,s-1,1); }

	s=random()%(10-5+1)+5;
	setValue(11,s,0);
	if (pm) { setValue(12,s-1,1); } else { setValue(12,s+1,1); }


	pm = random()%2;
	s=random()%(8-7+1)+7;
	setValue(s,6,0);
	if (pm)  { setValue(s+1,5,1); } else { setValue(s-1,5,1); }

	s=random()%(8-7+1)+7;
	setValue(s,9,0);
	if (pm) { setValue(s-1,10,1); } else { setValue(s+1,10,1); }
	
	// pick random start location
	s=random()%(m_dim-2)+1;
	setValue(0,s,0);
	setValue(1,s,0);
	// find an end location
	do {
		s=rand()%(m_dim-2)+1;
		if ( getValue(m_dim-2,s)==0 ) {
			setValue(m_dim-1,s,0);
		}
	} while (getValue(m_dim-1,s)==1);

}
