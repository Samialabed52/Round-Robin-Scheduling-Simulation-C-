#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

struct Job
{
    int pid, arrival, duration;
    Job *prev;
    Job *next;
    Job()
        : pid(0), arrival(0), duration(0), prev(nullptr), next(nullptr)
    {
    }
};

int main()
{
    string line{};
    vector<Job *>jobs;
    ifstream job("job.txt");
    if(!job.is_open())
    {
        cout << "File failed to open" << endl;
        return 0;
    }
    string temp_pid{};
    string temp_arrival{};
    string temp_duration{};
    while(getline(job, line))
    {
        Job *pointer = new Job();
        stringstream ss(line);
        getline(ss, temp_pid, ',');
        pointer->pid = stoi(temp_pid);
        getline(ss, temp_arrival, ',');
        pointer->arrival = stoi(temp_arrival);
        getline(ss, temp_duration);
        pointer->duration = stoi(temp_duration);
        jobs.push_back(pointer);
    }
    Job *wait_head = jobs.at(0);
    Job *wait_tail{};
    int tracker {0};
    for(size_t i {0}; i < jobs.size(); i++)
    {
        Job *pointer = jobs.at(i);
        wait_tail = jobs.at(i);
        tracker++;
        if(i == 0)
            pointer->prev = nullptr;
        else
            pointer->prev = jobs.at(i - 1);
        if(i + 1 == jobs.size())
            break;
        pointer->next = jobs.at(i + 1);
    }
    Job *ready_head = wait_head;
    Job *ready_tail = wait_head;
    Job *pointer = wait_head;
    int current_time{0};
    while(pointer->arrival == current_time)
    {
        if(pointer->pid == 0)
        {
            wait_head = pointer->next;
            pointer->next = nullptr;
            pointer = wait_head;
            pointer->prev = nullptr;
        }
        else
        {
            pointer->prev = ready_tail;
            ready_tail->next = pointer;
            wait_head = pointer->next;
            pointer->next = nullptr;
            ready_tail = pointer;
            pointer = wait_head;
            pointer->prev = nullptr;
        }
    }
    while(tracker != 0)
    {
        pointer = wait_head;
        if(wait_head != nullptr)
        {
            while(pointer->arrival == current_time && pointer != wait_tail)
            {
                ready_tail->next = pointer;
                pointer->prev = ready_tail;
                wait_head = pointer->next;
                pointer->next = nullptr;
                ready_tail = pointer;
                pointer = wait_head;
                pointer->prev = nullptr;
            }
        
            if(pointer->arrival == current_time && pointer == wait_tail)
            {
                ready_tail->next = wait_tail;
                wait_tail->prev = ready_tail;
                ready_tail = wait_tail;
                wait_tail = wait_head = nullptr;
            }
        }
        pointer = ready_head;
        if(pointer->duration > 5)
        {
            cout << "Job " << pointer->pid << ", scheduled for 5 ms" << endl;
            pointer->duration -= 5;
            current_time += 5;
            if(pointer->next != nullptr)
                ready_head = pointer->next;
            ready_tail->next = pointer;
            pointer->prev = ready_tail;
            ready_tail = pointer;
            pointer->next = nullptr;
        }
        else if(pointer->duration == 5)
        {
            cout << "Job " << pointer->pid << ", scheduled for 5 ms, completed" << endl;
            pointer->duration -= 5;
            current_time += 5;
            if(pointer->next != nullptr)
                ready_head = pointer->next;
            tracker--;
        }
        else
        {
            cout << "Job " << pointer->pid << ", completed" << endl;
            if(pointer->next != nullptr)
                ready_head = pointer->next;
            tracker--;
        }
    }
    cout << endl << "Total running time: " << current_time << endl << endl;
    
    return 0;
}