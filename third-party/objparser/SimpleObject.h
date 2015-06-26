#pragma once
#include "Vec3f.h"
#include <vector>

namespace SimpleOBJ
{
    template <typename T, int N> class Array 
    {
    public:
        enum {_len = N};
        typedef T t_Val; 
    public:
        Array() {}
        Array(const T& a, const T& b, const T& c) // add this to make it easy for me
        {
            _p[0] = a;
            _p[1] = b;
            _p[2] = c;
        }

        T& operator[] (int i)
        {
            // ASSERT(i>=0&&i<N);
            return _p[i];
        }
        const T& operator[] (int i) const 
        {
            // ASSERT(i>=0&&i<N);
            return _p[i];
        }

    protected:
        T _p[N];
    };

    class CSimpleObject
    {
    public:
        CSimpleObject(void);
        ~CSimpleObject(void);
        
    public:
        bool IsLoaded() { return m_pVertexList!=NULL;}

        void Destroy();
        bool LoadFromObj(const char* fn);
        bool SaveToObj(const char* fn);
        
    protected:
        bool Parse(FILE* fp);
        bool CheckParse(int nVertices,std::vector<Array<int,3> > & vecTriangles);

    public: // change it to public for easier visit
        int             m_nVertices;
        int             m_nTriangles;
        Vec3f*          m_pVertexList;
        Array<int,3>*   m_pTriangleList;
    };

}
