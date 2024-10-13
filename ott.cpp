#include <iostream>
#include <fstream> //creating,reading,writing into a file
#include <sstream> //parsing inputs and converting strings to numbers
#include <vector> //stores memory linearly and reizes itself
#include <stdexcept> //errors 

using namespace std;

// Define a Movie class
class Movie {
private:
    string title;
    string genre;
    int duration; // Duration in minutes
//making members private and accessing it in public functions.
public:
    Movie(const string& title, const string& genre, int duration) //constructor
        : title(title), genre(genre), duration(duration) {}

    
    string getTitle() const {
        return title;
    }

    string getGenre() const {
        return genre;
    }

    int getDuration() const {
        return duration;
    }
};

// Define a MoviesDatabase class using composition
class MoviesDatabase 
{
private:
    vector<Movie> movies;  //dynamically allocating memory

public:
    // Function to append a movie to the database
    void appendMovie(const Movie& movie) {
        movies.push_back(movie);
    }

    // Function to search for a movie by title
    Movie searchMovieByTitle(const string& title) const {
        for (const auto& movie : movies) {
            if (movie.getTitle() == title) {
                return movie;
            }
        }
        throw runtime_error("Movie not found.");
    }

    // Function to search for movies by genre
    vector<Movie> searchMoviesByGenre(const string& genre) const {
        vector<Movie> result;
        for (const auto& movie : movies) {
            if (movie.getGenre() == genre) {
                result.push_back(movie);
            }
        }
        return result;
    }

    // Function to save movies to a text file
    void saveToFile(const string& filename) const {
        ofstream outputFile(filename);
        for (const auto& movie : movies) {
            outputFile << movie.getTitle() << "," << movie.getGenre() << "," << movie.getDuration() << "\n";
        }
        cout << "Movies saved to file successfully!\n";
    }
};

// Function to display movies in a given genre
void displayMoviesByGenre(const MoviesDatabase& database, const string& genre)
{
    vector<Movie> foundMovies = database.searchMoviesByGenre(genre);

    if (foundMovies.empty()) {
        cout << "No movies found in the " << genre << " genre.\n";
    } else {
        cout << "Movies in the " << genre << " genre:\n";
        for (const auto& movie : foundMovies) {
            cout << movie.getTitle() << " (" << movie.getGenre() << ", Duration: " << movie.getDuration() << " minutes)\n";
        }
    }
}

int main() {
    try {
        MoviesDatabase database;

        // Load movies from a text file if it exists
        ifstream inputFile("movies.txt");
        if (inputFile) {
            string line;
            while (std::getline(inputFile, line)) {
                // Assuming the format of each line in the file is: Title,Genre,Duration
                istringstream iss(line);
                string title, genre;
                int duration;
                char comma;

                if (iss >> title >> ws >> comma >> genre >> ws >> comma >> duration) {
                    database.appendMovie(Movie(title, genre, duration));
                }
            }
        }

        int choice;
        do {
            cout << "\nChoose an option:\n";
            cout << "1. Append a movie\n";
            cout << "2. Search for a movie\n";
            cout << "3. Search for movies by genre\n";
            cout << "4. Save movies to file\n";
            cout << "0. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    string title, genre;
                    int duration;

                    cout << "\nEnter details for the Movie:\n";
                    cout << "Title: ";
                    cin.ignore(); // Clear the input buffer
                    getline(cin, title);
                    cout << "Genre: ";
                    getline(cin, genre);
                    cout << "Duration (in minutes): ";
                    cin >> duration;

                    database.appendMovie(Movie(title, genre, duration));
                    cout << "Movie appended successfully!\n";
                    break;
                }

                case 2: {
                    string searchTitle;
                    cout << "\nEnter the title of the movie to search: ";
                    cin.ignore(); // Clear the input buffer
                    getline(cin, searchTitle);

                    try {
                        Movie foundMovie = database.searchMovieByTitle(searchTitle);
                        cout << "Found Movie: " << foundMovie.getTitle() << " (" << foundMovie.getGenre()
                                  << ", Duration: " << foundMovie.getDuration() << " minutes)\n";
                    } catch (const exception& e) {
                        cerr << "Error: " << e.what() << endl;
                    }

                    break;
                }

                case 3: {
                    string searchGenre;
                    cout << "\nEnter the genre to search movies: ";
                    cin.ignore(); 
                    getline(cin, searchGenre);

                    displayMoviesByGenre(database, searchGenre);
                    break;
                }

                case 4: {
                    // Save movies to file
                    database.saveToFile("movies.txt");
                    break;
                }

                case 0:
                    cout << "Exiting the program. Goodbye!\n";
                    break;

                default:
                    cout << "Invalid choice. Please enter a valid option.\n";
            }

        } while (choice != 0);

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
