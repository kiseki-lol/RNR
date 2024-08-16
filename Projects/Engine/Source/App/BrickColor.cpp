#include <App/BrickColor.hpp>
#include <OGRE/RTShaderSystem/OgreShaderGenerator.h>

namespace RNR
{
    static BrickColor brickcolors[] = {
        // Placeholder color, should be replaced with Medium stone grey
        { -1, std::string("Placeholder"), Ogre::Vector3(255.0/255.0,0.0/255.0,255.0/255.0) },

        // Classic colors
        { 1, std::string("White"), Ogre::Vector3(242.0/255.0,243.0/255.0,243.0/255.0) },
        { 2, std::string("Grey"), Ogre::Vector3(161.0/255.0,165.0/255.0,162.0/255.0) },
        { 3, std::string("Light yellow"), Ogre::Vector3(249.0/255.0,233.0/255.0,153.0/255.0) },
        { 5, std::string("Brick yellow"), Ogre::Vector3(215.0/255.0,197.0/255.0,154.0/255.0) },
        { 6, std::string("Light green"), Ogre::Vector3(194.0/255.0,218.0/255.0,184.0/255.0) },
        { 9, std::string("Light reddish violet"), Ogre::Vector3(232.0/255.0,186.0/255.0,200.0/255.0) },
        { 11, std::string("Pastel Blue"), Ogre::Vector3(128.0/255.0,187.0/255.0,219.0/255.0) },
        { 12, std::string("Light orange brown"), Ogre::Vector3(203.0/255.0,132.0/255.0,66.0/255.0) },
        { 18, std::string("Nougat"), Ogre::Vector3(204.0/255.0,142.0/255.0,105.0/255.0) },
        { 21, std::string("Bright red"), Ogre::Vector3(196.0/255.0,40.0/255.0,28.0/255.0) },
        { 22, std::string("Med. reddish violet"), Ogre::Vector3(196.0/255.0,112.0/255.0,160.0/255.0) },
        { 23, std::string("Bright blue"), Ogre::Vector3(13.0/255.0,105.0/255.0,172.0/255.0) },
        { 24, std::string("Bright yellow"), Ogre::Vector3(245.0/255.0,205.0/255.0,48.0/255.0) },
        { 25, std::string("Earth orange"), Ogre::Vector3(98.0/255.0,71.0/255.0,50.0/255.0) },
        { 26, std::string("Black"), Ogre::Vector3(27.0/255.0,42.0/255.0,52.0/255.0) },
        { 27, std::string("Dark grey"), Ogre::Vector3(109.0/255.0,110.0/255.0,108.0/255.0) },
        { 28, std::string("Dark green"), Ogre::Vector3(40.0/255.0,127.0/255.0,71.0/255.0) },
        { 29, std::string("Medium green"), Ogre::Vector3(161.0/255.0,196.0/255.0,140.0/255.0) },
        { 36, std::string("Lg. Yellowich orange"), Ogre::Vector3(243.0/255.0, 207.0/255.0, 155.0/255.0) },
        { 37, std::string("Bright green"), Ogre::Vector3(75.0/255.0, 151.0/255.0, 75.0/255.0) },
        { 38, std::string("Dark orange"), Ogre::Vector3(160.0/255.0, 95.0/255.0, 53.0/255.0) },
        { 39, std::string("Light bluish violet"), Ogre::Vector3(193.0/255.0, 202.0/255.0, 222.0/255.0) },
        { 40, std::string("Transparent"), Ogre::Vector3(236.0/255.0, 236.0/255.0, 236.0/255.0) },
        { 41, std::string("Tr. Red"), Ogre::Vector3(205.0/255.0, 84.0/255.0, 75.0/255.0) },
        { 42, std::string("Tr. Lg blue"), Ogre::Vector3(193.0/255.0, 223.0/255.0, 240.0/255.0) },
        { 43, std::string("Tr. Blue"), Ogre::Vector3(123.0/255.0, 182.0/255.0, 232.0/255.0) },
        { 44, std::string("Tr. Yellow"), Ogre::Vector3(247.0/255.0, 241.0/255.0, 141.0/255.0) },
        { 45, std::string("Light blue"), Ogre::Vector3(180.0/255.0, 210.0/255.0, 228.0/255.0) },
        { 47, std::string("Tr. Flu. Reddish orange"), Ogre::Vector3(217.0/255.0, 133.0/255.0, 108.0/255.0) },
        { 48, std::string("Tr. Green"), Ogre::Vector3(132.0/255.0, 182.0/255.0, 141.0/255.0) },
        { 49, std::string("Tr. Flu. Green"), Ogre::Vector3(248.0/255.0, 241.0/255.0, 132.0/255.0) },
        { 50, std::string("Phosph. White"), Ogre::Vector3(236.0/255.0, 232.0/255.0, 222.0/255.0) },
        { 100, std::string("Light red"), Ogre::Vector3(238.0/255.0, 196.0/255.0, 182.0/255.0) },
        { 101, std::string("Medium red"), Ogre::Vector3(218.0/255.0, 134.0/255.0, 122.0/255.0) },
        { 102, std::string("Medium blue"), Ogre::Vector3(110.0/255.0, 153.0/255.0, 202.0/255.0) },
        { 103, std::string("Light grey"), Ogre::Vector3(199.0/255.0, 193.0/255.0, 183.0/255.0) },
        { 104, std::string("Bright violet"), Ogre::Vector3(107.0/255.0, 50.0/255.0, 124.0/255.0) },
        { 105, std::string("Br. yellowish orange"), Ogre::Vector3(226.0/255.0, 155.0/255.0, 64.0/255.0) },
        { 106, std::string("Bright orange"), Ogre::Vector3(218.0/255.0, 133.0/255.0, 65.0/255.0) },
        { 107, std::string("Bright bluish green"), Ogre::Vector3(0.0/255.0, 143.0/255.0, 156.0/255.0) },
        { 108, std::string("Earth yellow"), Ogre::Vector3(104.0/255.0, 92.0/255.0, 67.0/255.0) },
        { 110, std::string("Bright bluish violet"), Ogre::Vector3(67.0/255.0, 84.0/255.0, 147.0/255.0) },
        { 111, std::string("Tr. Brown"), Ogre::Vector3(191.0/255.0, 183.0/255.0, 177.0/255.0) },
        { 112, std::string("Medium bluish violet"), Ogre::Vector3(104.0/255.0, 116.0/255.0, 172.0/255.0) },
        { 113, std::string("Tr. Medi. reddish violet"), Ogre::Vector3(75.0/255.0, 151.0/255.0, 75.0/255.0) },
        { 115, std::string("Med. yellowish green"), Ogre::Vector3(199.0/255.0, 210.0/255.0, 60.0/255.0) },
        { 116, std::string("Med. bluish green"), Ogre::Vector3(85.0/255.0, 165.0/255.0, 175.0/255.0) },
        { 118, std::string("Light bluish green"), Ogre::Vector3(183.0/255.0, 215.0/255.0, 213.0/255.0) },
        { 119, std::string("Br. yellowish green"), Ogre::Vector3(164.0/255.0, 189.0/255.0, 71.0/255.0) },
        { 120, std::string("Lig. yellowish green"), Ogre::Vector3(217.0/255.0, 228.0/255.0, 167.0/255.0) },
        { 121, std::string("Med. yellowish orange"), Ogre::Vector3(231.0/255.0, 172.0/255.0, 88.0/255.0) },
        { 123, std::string("Br. reddish orange"), Ogre::Vector3(211.0/255.0, 111.0/255.0, 76.0/255.0) },
        { 124, std::string("Bright reddish violet"), Ogre::Vector3(146.0/255.0, 57.0/255.0, 120.0/255.0) },
        { 125, std::string("Light orange"), Ogre::Vector3(234.0/255.0, 184.0/255.0, 146.0/255.0) },
        { 126, std::string("Tr. Bright bluish violet"), Ogre::Vector3(165.0/255.0, 165.0/255.0, 203.0/255.0) },
        { 127, std::string("Gold"), Ogre::Vector3(220.0/255.0, 188.0/255.0, 129.0/255.0) },
        { 128, std::string("Dark nougat"), Ogre::Vector3(174.0/255.0, 122.0/255.0, 89.0/255.0) },
        { 131, std::string("Silver"), Ogre::Vector3(156.0/255.0, 163.0/255.0, 168.0/255.0) },
        { 133, std::string("Neon orange"), Ogre::Vector3(213.0/255.0, 115.0/255.0, 61.0/255.0) },
        { 134, std::string("Neon green"), Ogre::Vector3(216.0/255.0, 221.0/255.0, 86.0/255.0) },
        { 135, std::string("Sand blue"), Ogre::Vector3(116.0/255.0, 134.0/255.0, 157.0/255.0) },
        { 136, std::string("Sand violet"), Ogre::Vector3(135.0/255.0, 124.0/255.0, 144.0/255.0) },
        { 137, std::string("Medium orange"), Ogre::Vector3(224.0/255.0, 152.0/255.0, 100.0/255.0) },
        { 138, std::string("Sand yellow"), Ogre::Vector3(149.0/255.0, 138.0/255.0, 115.0/255.0) },
        { 140, std::string("Earth blue"), Ogre::Vector3(32.0/255.0, 58.0/255.0, 86.0/255.0) },
        { 141, std::string("Earth green"), Ogre::Vector3(39.0/255.0, 70.0/255.0, 45.0/255.0) },
        { 143, std::string("Tr. Flu. Blue"), Ogre::Vector3(207.0/255.0, 226.0/255.0, 247.0/255.0) },
        { 145, std::string("Sand blue metallic"), Ogre::Vector3(121.0/255.0, 136.0/255.0, 161.0/255.0) },
        { 146, std::string("Sand violet metallic"), Ogre::Vector3(149.0/255.0, 142.0/255.0, 163.0/255.0) },
        { 147, std::string("Sand yellow metallic"), Ogre::Vector3(147.0/255.0, 135.0/255.0, 103.0/255.0) },
        { 148, std::string("Dark grey metallic"), Ogre::Vector3(87.0/255.0, 88.0/255.0, 87.0/255.0) },
        { 149, std::string("Black metallic"), Ogre::Vector3(22.0/255.0, 29.0/255.0, 50.0/255.0) },
        { 150, std::string("Light grey metallic"), Ogre::Vector3(171.0/255.0, 173.0/255.0, 172.0/255.0) },
        { 151, std::string("Sand green"), Ogre::Vector3(120.0/255.0, 144.0/255.0, 130.0/255.0) },
        { 153, std::string("Sand red"), Ogre::Vector3(149.0/255.0, 121.0/255.0, 119.0/255.0) },
        { 154, std::string("Dark red"), Ogre::Vector3(123.0/255.0, 46.0/255.0, 47.0/255.0) },
        { 157, std::string("Tr. Flu. Yellow"), Ogre::Vector3(255.0/255.0, 246.0/255.0, 123.0/255.0) },
        { 158, std::string("Tr. Flu. Red"), Ogre::Vector3(225.0/255.0, 164.0/255.0, 194.0/255.0) },
        { 168, std::string("Gun metallic"), Ogre::Vector3(117.0/255.0, 108.0/255.0, 98.0/255.0) },
        { 176, std::string("Red flip/flop"), Ogre::Vector3(151.0/255.0, 105.0/255.0, 91.0/255.0) },
        { 178, std::string("Yellow flip/flop"), Ogre::Vector3(180.0/255.0, 132.0/255.0, 85.0/255.0) },
        { 179, std::string("Silver flip/flop"), Ogre::Vector3(137.0/255.0, 135.0/255.0, 136.0/255.0) },
        { 180, std::string("Curry"), Ogre::Vector3(215.0/255.0, 169.0/255.0, 75.0/255.0) },
        { 190, std::string("Fire Yellow"), Ogre::Vector3(249.0/255.0, 214.0/255.0, 46.0/255.0) },
        { 191, std::string("Flame yellowish orange"), Ogre::Vector3(232.0/255.0, 171.0/255.0, 45.0/255.0) },
        { 192, std::string("Reddish brown"), Ogre::Vector3(105.0/255.0, 64.0/255.0, 40.0/255.0) },
        { 193, std::string("Flame reddish orange"), Ogre::Vector3(207.0/255.0, 96.0/255.0, 36.0/255.0) },
        { 194, std::string("Medium stone grey"), Ogre::Vector3(163.0/255.0, 162.0/255.0, 165.0/255.0) },
        { 195, std::string("Royal blue"), Ogre::Vector3(70.0/255.0, 103.0/255.0, 164.0/255.0) },
        { 196, std::string("Dark Royal blue"), Ogre::Vector3(35.0/255.0, 71.0/255.0, 139.0/255.0) },
        { 198, std::string("Bright reddish lilac"), Ogre::Vector3(142.0/255.0, 66.0/255.0, 133.0/255.0) },
        { 199, std::string("Dark stone grey"), Ogre::Vector3(99.0/255.0, 95.0/255.0, 98.0/255.0) },
        { 208, std::string("Light stone grey"), Ogre::Vector3(229.0/255.0, 228.0/255.0, 223.0/255.0) },
        { 209, std::string("Dark Curry"), Ogre::Vector3(176.0/255.0, 142.0/255.0, 68.0/255.0) },
        { 210, std::string("Faded green"), Ogre::Vector3(112.0/255.0, 149.0/255.0, 120.0/255.0) },
        { 211, std::string("Turquoise"), Ogre::Vector3(121.0/255.0, 181.0/255.0, 181.0/255.0) },
        { 212, std::string("Light Royal blue"), Ogre::Vector3(159.0/255.0, 195.0/255.0, 233.0/255.0) },
        { 213, std::string("Medium Royal blue"), Ogre::Vector3(108.0/255.0, 129.0/255.0, 183.0/255.0) },
        { 216, std::string("Rust"), Ogre::Vector3(144.0/255.0, 76.0/255.0, 42.0/255.0) },
        { 217, std::string("Brown"), Ogre::Vector3(124.0/255.0, 92.0/255.0, 70.0/255.0) },
        { 218, std::string("Reddish lilac"), Ogre::Vector3(150.0/255.0, 112.0/255.0, 7159.0/255.0) },
        { 219, std::string("Lilac"), Ogre::Vector3(107.0/255.0, 98.0/255.0, 155.0/255.0) },
        { 220, std::string("Light lilac"), Ogre::Vector3(167.0/255.0, 169.0/255.0, 206.0/255.0) },
        { 221, std::string("Bright purple"), Ogre::Vector3(205.0/255.0, 98.0/255.0, 152.0/255.0) },
        { 222, std::string("Light purple"), Ogre::Vector3(228.0/255.0, 173.0/255.0, 200.0/255.0) },
        { 223, std::string("Light pink"), Ogre::Vector3(220.0/255.0, 144.0/255.0, 149.0/255.0) },
        { 224, std::string("Light brick yellow"), Ogre::Vector3(240.0/255.0, 213.0/255.0, 160.0/255.0) },
        { 225, std::string("Warm yellowish orange"), Ogre::Vector3(235.0/255.0, 184.0/255.0, 127.0/255.0) },
        { 226, std::string("Cool yellow"), Ogre::Vector3(253.0/255.0, 234.0/255.0, 141.0/255.0) },
        { 232, std::string("Dove blue"), Ogre::Vector3(125.0/255.0, 187.0/255.0, 221.0/255.0) },
        { 268, std::string("Medium lilac"), Ogre::Vector3(52.0/255.0, 43.0/255.0, 117.0/255.0) },
        { 301, std::string("Slime green"), Ogre::Vector3(80.0/255.0, 109.0/255.0, 84.0/255.0) },
        { 302, std::string("Smoky grey"), Ogre::Vector3(91.0/255.0, 93.0/255.0, 105.0/255.0) },
        { 303, std::string("Dark blue"), Ogre::Vector3(0.0/255.0, 16.0/255.0, 176.0/255.0) },
        { 304, std::string("Parsley green"), Ogre::Vector3(44.0/255.0, 101.0/255.0, 29.0/255.0) },
        { 305, std::string("Steel blue"), Ogre::Vector3(82.0/255.0, 124.0/255.0, 174.0/255.0) },
        { 306, std::string("Storm blue"), Ogre::Vector3(51.0/255.0, 88.0/255.0, 130.0/255.0) },
        { 307, std::string("Lapis"), Ogre::Vector3(16.0/255.0, 42.0/255.0, 220.0/255.0) },
        { 308, std::string("Dark indigo"), Ogre::Vector3(61.0/255.0, 21.0/255.0, 133.0/255.0) },
        { 309, std::string("Sea green"), Ogre::Vector3(52.0/255.0, 142.0/255.0, 64.0/255.0) },
        { 310, std::string("Shamrock"), Ogre::Vector3(91.0/255.0, 154.0/255.0, 76.0/255.0) },
        { 311, std::string("Fossil"), Ogre::Vector3(159.0/255.0, 161.0/255.0, 172.0/255.0) },
        { 312, std::string("Mulberry"), Ogre::Vector3(89.0/255.0, 34.0/255.0, 89.0/255.0) },
        { 313, std::string("Forest green"), Ogre::Vector3(31.0/255.0, 128.0/255.0, 29.0/255.0) },
        { 314, std::string("Cadet blue"), Ogre::Vector3(159.0/255.0, 173.0/255.0, 192.0/255.0) },
        { 315, std::string("Electric blue"), Ogre::Vector3(9.0/255.0, 137.0/255.0, 207.0/255.0) },
        { 316, std::string("Eggplant"), Ogre::Vector3(123.0/255.0, 0.0/255.0, 123.0/255.0) },
        { 317, std::string("Moss"), Ogre::Vector3(124.0/255.0, 156.0/255.0, 107.0/255.0) },
        { 318, std::string("Artichoke"), Ogre::Vector3(138.0/255.0, 171.0/255.0, 133.0/255.0) },
        { 319, std::string("Sage green"), Ogre::Vector3(185.0/255.0, 196.0/255.0, 177.0/255.0) },
        { 320, std::string("Ghost grey"), Ogre::Vector3(202.0/255.0, 203.0/255.0, 209.0/255.0) },
        { 321, std::string("Lilac"), Ogre::Vector3(167.0/255.0, 94.0/255.0, 155.0/255.0) },
        { 322, std::string("Plum"), Ogre::Vector3(123.0/255.0, 47.0/255.0, 123.0/255.0) },
        { 323, std::string("Olivine"), Ogre::Vector3(148.0/255.0, 190.0/255.0, 129.0/255.0) },
        { 324, std::string("Laurel green"), Ogre::Vector3(168.0/255.0, 189.0/255.0, 153.0/255.0) },
        { 325, std::string("Quill grey"), Ogre::Vector3(223.0/255.0, 223.0/255.0, 222.0/255.0) },
        { 327, std::string("Crimson"), Ogre::Vector3(151.0/255.0, 0.0/255.0, 0.0/255.0) },
        { 328, std::string("Mint"), Ogre::Vector3(177.0/255.0, 196.0/255.0, 177.0/255.0) },
        { 329, std::string("Baby blue"), Ogre::Vector3(152.0/255.0, 194.0/255.0, 219.0/255.0) },
        { 330, std::string("Carnation pink"), Ogre::Vector3(255.0/255.0, 152.0/255.0, 220.0/255.0) },
        { 331, std::string("Persimmon"), Ogre::Vector3(255.0/255.0, 89.0/255.0, 89.0/255.0) },
        { 332, std::string("Maroon"), Ogre::Vector3(117.0/255.0, 0.0/255.0, 0.0/255.0) },
        { 333, std::string("Gold"), Ogre::Vector3(239.0/255.0, 184.0/255.0, 56.0/255.0) },
        { 334, std::string("Daisy orange"), Ogre::Vector3(248.0/255.0, 217.0/255.0, 109.0/255.0) },
        { 335, std::string("Pearl"), Ogre::Vector3(231.0/255.0, 231.0/255.0, 236.0/255.0) },
        { 336, std::string("Fog"), Ogre::Vector3(199.0/255.0, 212.0/255.0, 228.0/255.0) },
        { 337, std::string("Salmon"), Ogre::Vector3(255.0/255.0, 148.0/255.0, 148.0/255.0) },
        { 338, std::string("Terra Cotta"), Ogre::Vector3(190.0/255.0, 104.0/255.0, 98.0/255.0) },
        { 339, std::string("Cocoa"), Ogre::Vector3(86.0/255.0, 36.0/255.0, 36.0/255.0) },
        { 340, std::string("Wheat"), Ogre::Vector3(241.0/255.0, 231.0/255.0, 199.0/255.0) },
        { 341, std::string("Buttermilk"), Ogre::Vector3(254.0/255.0, 243.0/255.0, 187.0/255.0) },
        { 342, std::string("Mauve"), Ogre::Vector3(224.0/255.0, 178.0/255.0, 208.0/255.0) },
        { 343, std::string("Sunrise"), Ogre::Vector3(212.0/255.0, 144.0/255.0, 189.0/255.0) },
        { 344, std::string("Tawny"), Ogre::Vector3(150.0/255.0, 85.0/255.0, 85.0/255.0) },
        { 345, std::string("Rust"), Ogre::Vector3(143.0/255.0, 76.0/255.0, 42.0/255.0) },
        { 346, std::string("Cashmere"), Ogre::Vector3(211.0/255.0, 190.0/255.0, 150.0/255.0) },
        { 347, std::string("Khaki"), Ogre::Vector3(226.0/255.0, 220.0/255.0, 188.0/255.0) },
        { 348, std::string("Lily white"), Ogre::Vector3(237.0/255.0, 234.0/255.0, 234.0/255.0) },
        { 349, std::string("Sea shell"), Ogre::Vector3(233.0/255.0, 218.0/255.0, 218.0/255.0) },
        { 350, std::string("Burgundy"), Ogre::Vector3(136.0/255.0, 62.0/255.0, 62.0/255.0) },
        { 351, std::string("Cork"), Ogre::Vector3(188.0/255.0, 155.0/255.0, 93.0/255.0) },
        { 352, std::string("Burlap"), Ogre::Vector3(199.0/255.0, 172.0/255.0, 120.0/255.0) },
        { 353, std::string("Beige"), Ogre::Vector3(202.0/255.0, 191.0/255.0, 163.0/255.0) },
        { 354, std::string("Oyster"), Ogre::Vector3(187.0/255.0, 179.0/255.0, 178.0/255.0) },
        { 355, std::string("Pine Cone"), Ogre::Vector3(108.0/255.0, 88.0/255.0, 75.0/255.0) },
        { 356, std::string("Fawn brown"), Ogre::Vector3(160.0/255.0, 132.0/255.0, 79.0/255.0) },
        { 357, std::string("Hurricane grey"), Ogre::Vector3(149.0/255.0, 137.0/255.0, 136.0/255.0) },
        { 358, std::string("Cloudy grey"), Ogre::Vector3(171.0/255.0, 168.0/255.0, 158.0/255.0) },
        { 359, std::string("Linen"), Ogre::Vector3(175.0/255.0, 148.0/255.0, 131.0/255.0) },
        { 360, std::string("Copper"), Ogre::Vector3(150.0/255.0, 103.0/255.0, 102.0/255.0) },
        { 361, std::string("Medium brown"), Ogre::Vector3(86.0/255.0, 66.0/255.0, 54.0/255.0) },
        { 362, std::string("Bronze"), Ogre::Vector3(126.0/255.0, 104.0/255.0, 63.0/255.0) },
        { 363, std::string("Flint"), Ogre::Vector3(105.0/255.0, 102.0/255.0, 92.0/255.0) },
        { 364, std::string("Dark taupe"), Ogre::Vector3(90.0/255.0, 76.0/255.0, 66.0/255.0) },
        { 365, std::string("Burnt Sienna"), Ogre::Vector3(106.0/255.0, 57.0/255.0, 9.0/255.0) },
        { 1001, std::string("Institutional white"), Ogre::Vector3(248.0/255.0, 248.0/255.0, 248.0/255.0) },
        { 1002, std::string("Mid gray"), Ogre::Vector3(205.0/255.0, 205.0/255.0, 205.0/255.0) },
        { 1003, std::string("Really black"), Ogre::Vector3(17.0/255.0, 17.0/255.0, 17.0/255.0) },
        { 1004, std::string("Really red"), Ogre::Vector3(255.0/255.0, 0.0/255.0, 0.0/255.0) },
        { 1005, std::string("Deep orange"), Ogre::Vector3(255.0/255.0, 176.0/255.0, 0.0/255.0) },
        { 1006, std::string("Alder"), Ogre::Vector3(180.0/255.0, 128.0/255.0, 255.0/255.0) },
        { 1007, std::string("Dusty Rose"), Ogre::Vector3(163.0/255.0, 75.0/255.0, 75.0/255.0) },
        { 1008, std::string("Olive"), Ogre::Vector3(193.0/255.0, 190.0/255.0, 66.0/255.0) },
        { 1009, std::string("New Yeller"), Ogre::Vector3(255.0/255.0, 255.0/255.0, 0.0/255.0) },
        { 1010, std::string("Really blue"), Ogre::Vector3(0/255.0, 0/255.0, 255.0/255.0) },
        { 1011, std::string("Navy blue"), Ogre::Vector3(0.0/255.0, 32.0/255.0, 96.0/255.0) },
        { 1012, std::string("Deep blue"), Ogre::Vector3(33.0/255.0, 84.0/255.0, 185.0/255.0) },
        { 1013, std::string("Cyan"), Ogre::Vector3(4.0/255.0, 175.0/255.0, 236.0/255.0) },
        { 1014, std::string("CGA brown"), Ogre::Vector3(170.0/255.0, 85.0/255.0, 0.0/255.0) },
        { 1015, std::string("Magenta"), Ogre::Vector3(170.0/255.0, 0.0/255.0, 170.0/255.0) },
        { 1016, std::string("Pink"), Ogre::Vector3(255.0/255.0, 102.0/255.0, 204.0/255.0) },
        { 1017, std::string("Deep orange"), Ogre::Vector3(255.0/255.0, 175.0/255.0, 0.0/255.0) },
        { 1018, std::string("Teal"), Ogre::Vector3(18.0/255.0, 238.0/255.0, 212.0/255.0) },
        { 1019, std::string("Toothpaste"), Ogre::Vector3(0.0/255.0, 255.0/255.0, 255.0/255.0) },
        { 1020, std::string("Lime green"), Ogre::Vector3(0.0/255.0, 255.0/255.0, 0.0/255.0) },
        { 1021, std::string("Camo"), Ogre::Vector3(58.0/255.0, 125.0/255.0, 21.0/255.0) },
        { 1022, std::string("Grime"), Ogre::Vector3(127.0/255.0, 142.0/255.0, 100.0/255.0) },
        { 1023, std::string("Lavender"), Ogre::Vector3(140.0/255.0, 91.0/255.0, 159.0/255.0) },
        { 1024, std::string("Pastel light blue"), Ogre::Vector3(175.0/255.0, 221.0/255.0, 255.0/255.0) },
        { 1025, std::string("Pastel orange"), Ogre::Vector3(255.0/255.0, 201.0/255.0, 201.0/255.0) },
        { 1026, std::string("Pastel violet"), Ogre::Vector3(177.0/255.0, 167.0/255.0, 255.0/255.0) },
        { 1027, std::string("Pastel blue-green"), Ogre::Vector3(159.0/255.0, 243.0/255.0, 233.0/255.0) },
        { 1028, std::string("Pastel green"), Ogre::Vector3(204.0/255.0, 255.0/255.0, 204.0/255.0) },
        { 1029, std::string("Pastel yellow"), Ogre::Vector3(255.0/255.0, 255.0/255.0, 204.0/255.0) },
        { 1030, std::string("Pastel brown"), Ogre::Vector3(255.0/255.0, 204.0/255.0, 153.0/255.0) },
        { 1031, std::string("Color"), Ogre::Vector3(98.0/255.0, 37.0/255.0, 209.0/255.0) },
        { 1032, std::string("Hot pink"), Ogre::Vector3(255.0/255.0, 0/255.0, 191.0/255.0) }
    };

    static bool bc_prep = false;

    BrickColor::BrickColor(int color_id, std::string color_name, Ogre::Vector3 color)
    {
        this->color_id = color_id;
        this->color_name = color_name;
        this->color_val = color;
    }

    void BrickColor::buildMaterial()
    {
        if(!bc_prep)
        {
            Ogre::RTShader::ShaderGenerator::getSingletonPtr()->createScheme("rtshader");

            bc_prep = true;
        }
        Ogre::MaterialPtr part_material = Ogre::MaterialManager::getSingletonPtr()->getByName("materials/PartInstanced");
        color_material = part_material->clone(Ogre::String("tmp_part/") + Ogre::StringConverter::toString(color_id));
        Ogre::RTShader::ShaderGenerator::getSingletonPtr()->cloneShaderBasedTechniques(*part_material, *color_material);
        Ogre::Technique* mat_tech = color_material->getTechnique(0);
        mat_tech->setLightingEnabled(true);
        mat_tech->setShadingMode(Ogre::ShadeOptions::SO_PHONG);
        Ogre::Pass* mat_pass = mat_tech->getPass(0);
        Ogre::TextureUnitState* part_texunit = mat_pass->getTextureUnitState(0);
        part_texunit->setColourOperationEx(Ogre::LayerBlendOperationEx::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, Ogre::ColourValue(color_val.x, color_val.y, color_val.z));

    }

    Ogre::Vector3 BrickColor::color(int brickcolor)
    {
        for(int i = 0; i < sizeof(brickcolors) / sizeof(BrickColor); i++)
        {
            if(brickcolors[i].color_id == brickcolor)
            {
                return brickcolors[i].color_val;
            }
        }
        return brickcolors[0].color_val;
    }

    Ogre::MaterialPtr BrickColor::material(int brickcolor)
    {
        for(int i = 0; i < sizeof(brickcolors) / sizeof(BrickColor); i++)
        {
            if(brickcolors[i].color_id == brickcolor)
            {
                if(!brickcolors[i].color_material)
                    brickcolors[i].buildMaterial();
                return brickcolors[i].color_material;
            }
        }
        if(!brickcolors[0].color_material)
            brickcolors[0].buildMaterial();
        return brickcolors[0].color_material;
    }

    bool BrickColor::valid(int brickcolor)
    {
        for(int i = 0; i < sizeof(brickcolors) / sizeof(BrickColor); i++)
        {
            if(brickcolors[i].color_id == brickcolor)
            {
                return true;
            }
        }
        return false;
    }

    std::string BrickColor::name(int brickcolor)
    {
        for(int i = 0; i < sizeof(brickcolors) / sizeof(BrickColor); i++)
        {
            if(brickcolors[i].color_id == brickcolor)
            {
                return brickcolors[i].color_name;
            }
        }
        return brickcolors[0].color_name;
    }
}