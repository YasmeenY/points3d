
// class Points2D is an array of 2d points. 
// It provides a clear interface for user to handle sequences of the 2d points
// and user doesn't need to care about memory.

#ifndef CSCI335_HOMEWORK2_POINTS2D_H_
#define CSCI335_HOMEWORK2_POINTS2D_H_

#include <array>
#include <iostream>
#include <cstddef>
#include <string>
#include <sstream>

namespace teaching_project {

template<typename Object>
class Points2D {
public:
    // Zero-parameter constructor. 
    // Set size to 0.
    Points2D() 
        : sequence_ { nullptr }
        , size_ { 0 }
    {}

    // Copy-constructor.
    Points2D(const Points2D &rhs) {
        size_ = rhs.size_;
        // allocate memory on the heap
        sequence_ = new std::array<Object, 2>[size_];
        // copy all points from `rhs` sequence to this sequence
        for(size_t i = 0; i < size_; i++) {
            sequence_[i][0] = rhs.sequence_[i][0];
            sequence_[i][1] = rhs.sequence_[i][1];
        }
    }

    // Copy-assignment
    Points2D& operator=(const Points2D &rhs) {
        if(this != &rhs) { // prevent self-assignment
            // invoke copy ctor : copy rhs to be able modify it 
            Points2D copy { rhs };
            // swaps using move semantic
            std::swap(*this, copy);
        }
        return *this;
    };

    // Move-constructor. 
    Points2D(Points2D &&rhs) 
        : sequence_ { rhs.sequence_ } // now 2 pointers points to this memory
        , size_ { rhs.size_ }
    {
        // reset rhs
        rhs.sequence_ = nullptr; // now only 1 pointer points to the sequence
        rhs.size_ = 0;
    };

    // Move-assignment.
    Points2D& operator=(Points2D &&rhs) {
        if(this != &rhs) { // prevent self-assignment
            // just swap data as we don't care what state @rhs will have after std::move, 
            // we only need to be sure that it will be handled correctly when destructor is called 
            std::swap(size_, rhs.size_);
            std::swap(sequence_, rhs.sequence_);
        }
        return *this;
    }

    ~Points2D() {
        if(sequence_) {
            // clean memory
            delete[] sequence_;
        } 
    };

    // End of big-five.

    // One parameter constructor.
    // Create sequence with 1 point
    Points2D(const std::array<Object, 2>& item) {
        size_ = 1;
        sequence_ = new std::array<Object, 2>[size_];
        sequence_[0][0] = item[0];
        sequence_[0][1] = item[1];
    }

    // Read a chain from standard input.
    void ReadPoints2D() {
        // Part of code included (without error checking).
        std::string input_line;
        std::getline(std::cin, input_line); 
        std::stringstream input_stream(input_line);
        if (input_line.empty()) return;
        // Read size of sequence (an integer).
        int size_of_sequence;
        input_stream >> size_of_sequence;
        // Allocate space for sequence.
        Points2D temp;
        temp.size_ = size_of_sequence;
        temp.sequence_ = new std::array<Object, 2>[temp.size_];

        Object token;
        for (int i = 0; input_stream >> token; ++i) {
            // Read coordinates. (Each first coordinate is `X` and second - `Y`)
            temp.sequence_[i / 2][i % 2] = token;
        }
        
        *this = std::move(temp);
    }

    size_t size() const {
        return size_;
    }

    // @location: an index to a location in the given sequence.
    // @returns the point at @location.
    // const version.
    // abort() if out-of-range.
    const std::array<Object, 2>& operator[](size_t location) const { 
        if(location >= size_) {
            abort(); // out_of_range
        }
        return sequence_[location];
    }

	//  @c1: A sequence.
	//  @c2: A second sequence.
	//  @return their sum. If the sequences are not of the same size, append the
	//    result with the remaining part of the larger sequence.
	friend Points2D operator+(const Points2D &c1, const Points2D &c2) {
		Points2D points;
        points.size_ = c1.size_ > c2.size_? c1.size_ : c2.size_;
        points.sequence_ = new std::array<Object, 2>[points.size_];
        for(size_t i = 0; i < points.size_; i++) {
            // add 'X' component
            points.sequence_[i][0] = c1.size_ > i? c1[i][0] : 0;
            points.sequence_[i][0] += c2.size_ > i? c2[i][0] : 0;

            // add 'Y' component
            points.sequence_[i][1] = c1.size_ > i? c1[i][1] : 0;
            points.sequence_[i][1] += c2.size_ > i? c2[i][1] : 0;
        }
        return points;
	}

	// Overloading the << operator.
	friend std::ostream &operator<<(std::ostream &out, const Points2D &some_points2d) {
        for(size_t i = 0; i < some_points2d.size_; i++) {
            out << "(" << some_points2d.sequence_[i][0] << ", " << some_points2d.sequence_[i][1] << ") ";
        }
        if(some_points2d.size_ == 0) {
            out << "()"; // expected_output.txt force to print this 
        }
        out << std::endl;
        return out;
	}
 
private:

    // Sequence of points. 
    std::array<Object, 2> *sequence_;
    // Size of the sequence.
    size_t size_;
};

}  // namespace teaching_project
#endif // CSCI_335_HOMEWORK2_POINTS2D_H_
