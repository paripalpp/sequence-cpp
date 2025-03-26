#include <sequence-cpp/sequence.hpp>

namespace sequence{

    SequenceSeries::SequenceSeries(std::vector<Sequence::ptr> seqs): seqs(seqs) {
        current = seqs.begin();
    }

    SequenceSeries::SequenceSeries(std::initializer_list<Sequence::ptr> seqs_list): seqs(seqs_list) {
        current = seqs.begin();
    }

    void SequenceSeries::start(){
        current = seqs.begin();
        if(this->is_finished()) return;
        current->get()->start();
    }
    

    void SequenceSeries::stop(){
        if(this->is_finished()) return;
        current->get()->stop();
        current = seqs.begin();
    }
    

    void SequenceSeries::task(){
        if(this->is_finished()) return;
        current->get()->task();
        if(current->get()->is_finished()){
            start_next();
        }
    }
    

    bool SequenceSeries::is_finished(){
        return current == seqs.end();
    }
    

    void SequenceSeries::start_next(){
        if(this->is_finished()) return;
        current++;
        if(current != seqs.end()){
            current->get()->start();
        }
    }
    

    SequenceParallel::SequenceParallel(std::vector<Sequence::ptr> seqs): seqs(seqs) {}
    

    SequenceParallel::SequenceParallel(std::initializer_list<Sequence::ptr> seqs): seqs(seqs) {}
    

    void SequenceParallel::start(){
        finished_count = 0;
        for(auto &seq: seqs){
            seq->start();
        }
    }
    

    void SequenceParallel::stop(){
        for(auto &seq: seqs){
            seq->stop();
        }
    }
    

    void SequenceParallel::task(){
        finished_count = 0;
        for(auto &seq: seqs){
            if(!seq->is_finished()){
                seq->task();
            }else{
                finished_count++;
            }
        }
    }
    

    bool SequenceParallel::is_finished(){
        return finished_count == seqs.size();
    }
    
    Loop::Loop(Sequence::ptr seq): seq(seq) {}

    void Loop::start(){
        seq->start();
        finished = false;
    }

    void Loop::stop(){
        seq->stop();
        finished = true;
    }

    void Loop::task(){
        seq->task();
        if(seq->is_finished()){
            seq->start();
        }
    }

    bool Loop::is_finished(){
        return finished;
    }

    While::While(std::function<bool()> condition, Sequence::ptr seq): condition(condition), seq(seq) {}

    void While::start(){
        if(condition()){
            seq->start();
            finished = false;
        }else{
            finished = true;
        }
    }

    void While::stop(){
        seq->stop();
        finished = true;
    }

    void While::task(){
        if(!finished){
            seq->task();
            if(seq->is_finished()){
                if(condition()){
                    seq->start();
                }else{
                    finished = true;
                }
            }
        }
    }

    bool While::is_finished(){
        return finished;
    }
}