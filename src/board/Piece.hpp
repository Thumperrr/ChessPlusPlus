#ifndef ChessPieceBaseClass_HeaderPlusPlus
#define ChessPieceBaseClass_HeaderPlusPlus

//Warnings:
//Changing the dimensions or layout of the piece texture (the actual png)
//will break the use of a piece and it's derived classes
//Notes:
//A piece's suit determines what part of the texture it belongs to:
//textureX = givenValX
//textureY = givenValY + ( this->suit == WHITE ? 0 : 80 )

#include "util/Position.hpp"
#include "Board.hpp"

#include <set>
#include <iostream>
#include <typeinfo>

namespace chesspp
{
    namespace board
    {
        enum class Suit //needs to be abstracted
        {
            WHITE,
            BLACK
        };
        std::ostream &operator<<(std::ostream &os, Suit c)
        {
            switch(c)
            {
            case Suit::WHITE:
                os << "White";
                break;
            case Suit::BLACK:
                os << "Black";
                break;
            }
            os << " suit";
        };

        class Piece
        {
        public:
            using Position_t = Board::Position_t;
            using PosList_t = std::set<Position_t>;
        private:
            Suit s;
            PosList_t traj;
            Position_t p;
        public:
            //const aliases for deriving classes
            Suit const &suit;            //Which suit the chess piece is
            PosList_t const &trajectory; //The list of possible Positions
            Position_t const &pos;       //The position on the baord this piece is

            Board &b; //The board this piece belongs to

            Piece(Board &b, Position_t const &pos, Suit s)
            : b(b)
            , p(pos)
            , s(s)
            , suit(s)
            , trajectory(traj)
            , pos(p)
            {
                std::clog << "Creation of " << *this << std::endl;
            }
            virtual ~Piece() = default;

            //non-virtual, calls calcTrajectory(), which should call addTrajectory() for each possible tile
            void makeTrajectory()
            {
                traj.clear();
                calcTrajectory();
            }
        protected:
            //should call addTrajectory() for each calculated trajectory
            virtual void calcTrajectory() = 0;
            //deriving classes should call this from makeTrajectory to add a calculated trajectory tile
            void addTrajectory(Position_t tile)
            {
                if(b.valid(tile))
                {
                    traj.insert(tile);
                }
                else
                {
                    std::clog << "Invalid tile " << tile << " calculated for trajectory by " << *this << std::endl;
                }
            }
            //further deriving classes can call this to remove a trajectory calculated by their parent class
            void removeTrajectory(Position_t tile)
            {
                traj.erase(tile);
            }

        public:
            //Sets the piece position as instructed by the board and recalculates the trajectory
            void move(Position_t const &to)
            {
                Position_t from = std::move(pos);
                pos = to;
                moveAnimation(from, to);
                makeTrajectory();
            }
        private: //intentionally private, not protected
            //Called by move(), plays the animation for moving
            virtual void moveAnimation(Position_t const &from, Position_t const &to) = 0;
        public:

            friend std::ostream &operator<<(std::ostream &os, Piece const &p)            
            {
                return os << "Piece (" << typeid(p).name() << ") " << p.suit << " at " << p.pos;
            }
        };
    }
}

#endif
