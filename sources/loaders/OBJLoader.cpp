//
// Created by Владимир Шелестов aka DarthShelL on 05.05.2022.
//


#include "OBJLoader.h"

void
OBJLoader::Load(const char *filename, vector<float> &vertices, vector<float> &normals, vector<GLushort> &elements) {
    ifstream in(filename, ios::in);
    if (!in) {
        cerr << "Cannot open " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(in, line)) {
        if (line.substr(0, 2) == "v ") {
            istringstream s(line.substr(2));
            float x, y, z;
            s >> x;
            s >> y;
            s >> z;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        } else if (line.substr(0, 3) == "vn ") {
            istringstream s(line.substr(3));
            float x, y, z;
            s >> x;
            s >> y;
            s >> z;
            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);
        } else if (line.substr(0, 2) == "f ") {
            string delimiter = " ";
            string text = line.substr(2);
            size_t pos = 0;
            vector<string> v;
            while ((pos = text.find(delimiter)) != string::npos) {
                v.push_back(text.substr(0, pos));
                text.erase(0, pos + delimiter.length());
            }
            if (text.size() > 0)
                v.push_back(text);

            for (int i = 0; i < v.size(); i++) {
                string delimiter = "/";
                string text = v[i];
                size_t pos = 0;
                vector<string> final;
                while ((pos = text.find(delimiter)) != string::npos) {
                    final.push_back(text.substr(0, pos));
                    text.erase(0, pos + delimiter.length());
                }
                if (text.size() > 0)
                    final.push_back(text);

                // 0 - vertex
                GLushort index = stoi(final[0]);
                // 1 - texture
                // 2 - normal
                GLushort n_index = stoi(final[2]);

                index--;
                n_index--;

                elements.push_back(index);

                index *= 6;
                n_index *= 3;

                vertices[index + 3] = normals[n_index];
                vertices[index + 4] = normals[n_index + 1];
                vertices[index + 5] = normals[n_index + 2];
            }
        }
        /* anything else is ignored */
    }

    //smooth shading
//    normals.resize(vertices.size());
//    for (int i = 0; i < elements.size(); i++) {
//
//    }
//
//    cout << "Vertices:" << vertices.size() << endl;
//    for (int i = 0; i < vertices.size(); i++) {
//        cout << vertices[i] << " ";
//        if ((i + 1) % 6 == 0)
//            cout << endl;
//    }
//    cout << endl;
//    cout << "Normals:" << normals.size() << endl;
//    for (int i = 0; i < normals.size(); i++) {
//        cout << normals[i] << " ";
//        if ((i + 1) % 3 == 0)
//            cout << endl;
//    }
//    cout << endl;
//    cout << "Elements:" << elements.size() << endl;
//    for (int i = 0; i < elements.size(); i++) {
//        cout << elements[i] << " ";
//        if ((i + 1) % 3 == 0)
//            cout << endl;
//    }
//    cout << endl;
}
