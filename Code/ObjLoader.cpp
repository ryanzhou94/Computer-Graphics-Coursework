//
//  ObjLoader.cpp
//  G53GRA.Framework
//
//  Created by 周泽炜 on 2020/11/23.
//  Copyright © 2020 w.o.c.ward. All rights reserved.
//

#include "ObjLoader.hpp"
#include <fstream>
#include <sstream>
using namespace std;

ObjLoader::ObjLoader(string filename)
{
    string line;                    // file content
    fstream f;                      // file stream
    f.open(filename, ios::in);      // open the file

    // check status
    if (!f.is_open()){
        cout << "Something Went Wrong When Opening Objfiles" << endl;
    }

    // start to read the file
    while (!f.eof()){
        // read one line
        getline(f, line);
        vector<string> parameters;
        string tailMark = " ";
        string ans = "";
        line = line.append(tailMark);
        
        // read the whole line and save elements in 'parameters'
        for (int i = 0; i < line.length(); i++) {
            char ch = line[i];
            if (ch != ' ') {
                ans+=ch;
            }
            else {
                parameters.push_back(ans);
                ans = "";
            }
        }

        if (parameters.size() != 4) {
            cout << "the size is not correct" << endl;
        }

        else {
            if (parameters[0] == "v") {
                // point/vertex
                vector<GLfloat> Point;
                for (int i = 1; i < 4; i++) {
                    GLfloat xyz = atof(parameters[i].c_str());
                    Point.push_back(xyz);
                }
                vSets.push_back(Point);
            }

            else if (parameters[0] == "f") {
                // faces/面
                // there is an old version of .obj file and a new version of .obj file. the number represents the index
                // old: f 1 2 3
                // new: f 1//2 5//7 11//25
                // the following code applies to the old version
                vector<GLint> vIndexSets;

                for (int i = 1; i < 4; i++){
                    string x = parameters[i];
                    string ans = "";
                    for (int j = 0; j < x.length(); j++) {
                        char ch = x[j];
                        if (ch != '/') {
                            ans += ch;
                        }
                        else {
                            break;
                        }
                    }
                    GLint index = atof(ans.c_str());
                    index = index--;
                    vIndexSets.push_back(index);
                }
                fSets.push_back(vIndexSets);
            }

            else if (parameters[0] == "vn") {
                vector<GLfloat>vnValueSets;
                for (int i = 1; i < 4; i++) {
                    GLfloat vnxyz = atof(parameters[i].c_str());
                    vnValueSets.push_back(vnxyz);
                }
                vnSets.push_back(vnValueSets);
            }

            else if (parameters[0] == "vt") {
                vector<GLfloat>vtValueSets;
                for (int i = 1; i < 4; i++) {
                    GLfloat vnxyz = atof(parameters[i].c_str());
                    vtValueSets.push_back(vnxyz);
                }
                vtSets.push_back(vtValueSets);
            }

        }
    }
    f.close();
}

void ObjLoader::Draw()
{
    glBegin(GL_TRIANGLES);
        for (int i = 0; i < fSets.size(); i++) {
            GLfloat VN[3];
            GLfloat SV1[3];
            GLfloat SV2[3];
            GLfloat SV3[3];

            if ((fSets[i]).size() != 3) {
                cout << "the fSetsets_Size is not correct" << endl;
            }
            else {
                    GLint firstVertexIndex = (fSets[i])[0] - 1;
                    GLint secondVertexIndex = (fSets[i])[1] - 1;
                    GLint thirdVertexIndex = (fSets[i])[2] - 1;

                    SV1[0] = (vSets[firstVertexIndex])[0];
                    SV1[1] = (vSets[firstVertexIndex])[1];
                    SV1[2] = (vSets[firstVertexIndex])[2];

                    SV2[0] = (vSets[secondVertexIndex])[0];
                    SV2[1] = (vSets[secondVertexIndex])[1];
                    SV2[2] = (vSets[secondVertexIndex])[2];

                    SV3[0] = (vSets[thirdVertexIndex])[0];
                    SV3[1] = (vSets[thirdVertexIndex])[1];
                    SV3[2] = (vSets[thirdVertexIndex])[2];


                    GLfloat vec1[3], vec2[3], vec3[3];
                    //(x2-x1,y2-y1,z2-z1)
                    vec1[0] = SV1[0] - SV2[0];
                    vec1[1] = SV1[1] - SV2[1];
                    vec1[2] = SV1[2] - SV2[2];

                    //(x3-x2,y3-y2,z3-z2)
                    vec2[0] = SV1[0] - SV3[0];
                    vec2[1] = SV1[1] - SV3[1];
                    vec2[2] = SV1[2] - SV3[2];

                    //(x3-x1,y3-y1,z3-z1)
                    vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
                    vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
                    vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

                    GLfloat D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));

                    VN[0] = vec3[0] / D;
                    VN[1] = vec3[1] / D;
                    VN[2] = vec3[2] / D;
               
                    glNormal3f(VN[0], VN[1], VN[2]);

                    glVertex3f(SV1[0], SV1[1], SV1[2]);
                    glVertex3f(SV2[0], SV2[1], SV2[2]);
                    glVertex3f(SV3[0], SV3[1], SV3[2]);
            }
        }
        glEnd();
}

