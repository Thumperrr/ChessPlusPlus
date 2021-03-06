#ifndef ChessPlusPlusBoardConfigurationManagerClass_HeaderPlusPlus
#define ChessPlusPlusBoardConfigurationManagerClass_HeaderPlusPlus

#include "Configuration.hpp"
#include "GraphicsConfig.hpp"
#include "util/Position.hpp"

#include <string>
#include <cstdint>
#include <utility>
#include <map>

namespace chesspp
{
    namespace config
    {
        class BoardConfig : public Configuration
        {
        public:
            using BoardSize_t = std::uint8_t;
            using CellSize_t = std::uint16_t;
            using Position_t = util::Position<BoardSize_t>; //Position type is based on Board Size type
            using PieceClass_t = std::string;
            using SuitClass_t = std::string;
            using Layout_t = std::map<Position_t, std::pair<PieceClass_t, SuitClass_t>>;
            using Textures_t = std::map<BoardConfig::SuitClass_t, std::map<BoardConfig::PieceClass_t, std::string>>;
        private:
            BoardSize_t board_width, board_height;
            CellSize_t cell_width, cell_height;
            Layout_t layout;
            Textures_t textures;

        public:
            BoardConfig(GraphicsConfig const &gfx)
            : Configuration("config/board.json")
            , board_width  (reader()["chesspp"]["board"]["width"]      )
            , board_height (reader()["chesspp"]["board"]["height"]     )
            , cell_width   (reader()["chesspp"]["board"]["cell width"] )
            , cell_height  (reader()["chesspp"]["board"]["cell height"])
            {
                auto pieces = reader()["chesspp"]["board"]["pieces"];
                auto suits  = reader()["chesspp"]["board"]["suits"];
                for(BoardSize_t r = 0; r < board_height; ++r)
                {
                    for(BoardSize_t c = 0; c < board_width; ++c)
                    {
                        auto piece = pieces[r][c];
                        auto suit  = suits [r][c];
                        if(piece.type() != json_null) //it is OK if suit is null
                        {
                            layout[{c, r}] = std::make_pair<PieceClass_t, SuitClass_t>(piece, suit);
                        }
                    }
                }

                auto const &tex = gfx.spritePaths("board", "pieces");
                for(auto const &suit : tex)
                {
                    for(auto const &piece : suit.second.object())
                    {
                        textures[suit.first][piece.first] = gfx.normalize(Textures_t::mapped_type::mapped_type(piece.second));
                    }
                }
            }
            virtual ~BoardConfig() = default;

            BoardSize_t       boardWidth   () const noexcept { return board_width;  }
            BoardSize_t       boardHeight  () const noexcept { return board_height; }
            Layout_t const   &initialLayout() const noexcept { return layout;       }
            CellSize_t        cellWidth    () const noexcept { return cell_width;   }
            CellSize_t        cellHeight   () const noexcept { return cell_height;  }
            Textures_t const &texturePaths () const noexcept { return textures;     }

            template<typename... Args>
            util::JsonReader::NestedValue metadata(Args const &... path) const
            {
                return reader.navigate("chesspp", "board", "metadata", path...);
            }
        };
    }
}

#endif
