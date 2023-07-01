#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <stdexcept>

// install gnuplot
// set environment  (e.g.) PATH = C:\Program files\gnuplot\bin

class gnuplot_pipe
{
public:
    gnuplot_pipe()
    {
#ifdef WIN32
        pipe = _popen("gnuplot", "w");// -persist
#else
        pipe = popen("gnuplot", "w");
#endif
        if (pipe == nullptr)
            throw std::runtime_error(std::string("could not open pipe").c_str());

        fprintf(pipe, "set term wx\n");
    }
    ~gnuplot_pipe()
    {
        if (pipe == nullptr)
            return;
        fprintf(pipe, "exit\n");
#ifdef WIN32
        _pclose(pipe);
#else
        pclose(pipe);
#endif
    }
    void command(std::string str)
    {
        if (str.size() > 0)
            if (str.back() != '\n')
                str += "\n";
        fprintf(pipe, str.c_str());
    }
    template<typename T>
    void add_array(std::vector<T>& data)
    {
        for (size_t i = 0; i < data.size(); i++)
        {
            double f = static_cast<double>(data[i]);
            fprintf(pipe, "%f\n", f);
        }
        fprintf(pipe, "e\n");
    }
    template<typename T>
    void add_array(std::vector<std::vector<T>>& data)
    {
        for (size_t i = 0; i < data.size(); i++)
        {
            for (int j = 0; j < data[i].size(); j++)
            {
                float f = static_cast<double>(data[i][j]);
                fprintf(pipe, "%f ", f);
            }
            fprintf(pipe, "\n");
        }
        fprintf(pipe, "e\n");
    }
    void flush()
    {
        fflush(pipe);
    }
protected:
    FILE* pipe = nullptr;
};

// sample code
//int main()
//{
//    try
//    {
//        gnuplot_pipe gp;
//
//        gp.command("plot '-' with lines\n");
//
//        std::vector<float> dat0;
//        for (int i = 0; i < 50; i++)
//            dat0.push_back(std::sin(0.1 * i));
//        gp.add_array(dat0);
//        gp.flush();
//    }
//    catch (std::exception& e)
//    {
//        std::cout << e.what();
//    }
//}
