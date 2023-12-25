

#include <Pipeline/CSVToLevelLoader.hpp>

#include <AllegroFlare/CSVParser.hpp>
#include <AllegroFlare/Logger.hpp>
#include <AllegroFlare/UsefulPHP.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace Pipeline
{


CSVToLevelLoader::CSVToLevelLoader()
   : AllegroFlare::CSVParser()
   , csv_full_path("[unset-csv_full_path]")
   , levels({})
   , loaded(false)
{
}


CSVToLevelLoader::~CSVToLevelLoader()
{
}


void CSVToLevelLoader::set_csv_full_path(std::string csv_full_path)
{
   this->csv_full_path = csv_full_path;
}


std::string CSVToLevelLoader::get_csv_full_path() const
{
   return csv_full_path;
}


std::map<std::string, Pipeline::Gameplay::Level> CSVToLevelLoader::get_levels()
{
   if (!(loaded))
   {
      std::stringstream error_message;
      error_message << "[CSVToLevelLoader::get_levels]: error: guard \"loaded\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("CSVToLevelLoader::get_levels: error: guard \"loaded\" not met");
   }
   return levels;
}

bool CSVToLevelLoader::level_exists(std::string level_identifier)
{
   if (!(loaded))
   {
      std::stringstream error_message;
      error_message << "[CSVToLevelLoader::level_exists]: error: guard \"loaded\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("CSVToLevelLoader::level_exists: error: guard \"loaded\" not met");
   }
   return (levels.find(level_identifier) != levels.end());
}

Pipeline::Gameplay::Level CSVToLevelLoader::get_copy_of_level(std::string level_identifier)
{
   if (!(loaded))
   {
      std::stringstream error_message;
      error_message << "[CSVToLevelLoader::get_copy_of_level]: error: guard \"loaded\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("CSVToLevelLoader::get_copy_of_level: error: guard \"loaded\" not met");
   }
   if (!(level_exists(level_identifier)))
   {
      std::stringstream error_message;
      error_message << "[CSVToLevelLoader::get_copy_of_level]: error: guard \"level_exists(level_identifier)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("CSVToLevelLoader::get_copy_of_level: error: guard \"level_exists(level_identifier)\" not met");
   }
   return levels[level_identifier];
}

std::string CSVToLevelLoader::validate_key_and_return(std::map<std::string, std::string>* extracted_row, std::string key)
{
   if (extracted_row->count(key) == 0)
   {
      AllegroFlare::Logger::throw_error(
        "SurviveTheCity::DeckFactory::validate_key_and_return",
        "key \"" + key+ "\" does not exist."
     );
   }
   return extracted_row->operator[](key);
}

void CSVToLevelLoader::load()
{
   if (!((!loaded)))
   {
      std::stringstream error_message;
      error_message << "[CSVToLevelLoader::load]: error: guard \"(!loaded)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("CSVToLevelLoader::load: error: guard \"(!loaded)\" not met");
   }
   // Obtain the content from the file and parse it to extractable data
   std::string content = AllegroFlare::php::file_get_contents(csv_full_path);
   if (content.empty()) throw std::runtime_error("empty file content");
   AllegroFlare::CSVParser csv_parser;
   csv_parser.set_raw_csv_content(content);
   csv_parser.parse();
   csv_parser.assemble_column_headers(2);

   // Load the parsed data to Level objects
   int first_physical_row = csv_parser.get_num_header_rows();
   for (std::map<std::string, std::string> &extracted_row : csv_parser.extract_all_rows())
   {
      // Pull out the variables
      std::string identifier =
         validate_key_and_return(&extracted_row, "level_identifier");
      std::string world_model_obj =
         validate_key_and_return(&extracted_row, "world__model_obj_filename");
      std::string world_texture_filename =
         validate_key_and_return(&extracted_row, "world__model_texture_filename");

      // Pass along the variables to the result object
      Pipeline::Gameplay::Level level;
      //level.set
      levels.insert({ identifier, level });
   }


   loaded = true;
   return;
}


} // namespace Pipeline

