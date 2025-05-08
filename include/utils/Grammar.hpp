#pragma once

#include <string>                       
#include <vector>                      
#include <unordered_map>               
#include <unordered_set>               
#include <utility>                     
#include "Types.hpp"                   

namespace gracfl 
{
	/**
     * @class Grammar
     * @brief Loads a normalized context-free grammar and provides quick lookup for
     *        epsilon, unary, and binary productions.
     *
     * The Grammar class parses a text file where each line defines a grammar rule
     * (with 1–3 symbols) and builds indices for efficient rule lookup:
     * - grammar1_: epsilon productions (A ::= )
     * - grammar2_: unary productions (A ::= B)
     * - grammar3_: binary productions (A ::= B C)
     */
	class Grammar 
	{
	public:
		/// total number of unique edge labels
		uint labelSize_ = 0;
		///  Path to the grammar definition file
		std::string grammarFilePath_;

		// Raw rules:
		/// epsilon production rules
		std::vector<std::vector<uint>> grammar1_;	
		/// production rules of type A = B
		std::vector<std::vector<uint>> grammar2_;	
		/// production rules of type A = BC
		std::vector<std::vector<uint>> grammar3_;	

		// Indexed for fast lookup:
		/// Flags for presence of epsilon rules per symbol
		std::vector<bool> grammar1index_;	
		/// Map B -> all As for A ::= B 
		std::vector<std::vector<uint>> grammar2index_;
		/// Map (B,C) -> all As for A ::= B C
		std::vector<std::vector<uint>> grammar3index_;
		/// Map B -> [(A,C)] for A ::= B C
		std::vector<std::vector<std::pair<uint, uint>>> grammar3indexLeft_;	
		// Map C -> [(A,B)] for A ::= B C 
		std::vector<std::vector<std::pair<uint, uint>>> grammar3indexRight_;

		// Symbol tables:
		/// Map label string -> ID
		std::unordered_map<std::string, uint> hashSym_;
		/// Map ID -> label string
		std::unordered_map<uint, std::string> hashSymRev_;
		/// All symbols on LHS of productions
		std::vector<uint> leftLabels_;
		/// All labels appearing in grammar
		std::unordered_set<std::string> allLabels_;
		/// Context-specific labels (e.g., call/return)
		std::vector<uint> contextLabels_;

		/**
         * @brief Reads the grammar file and populates rule and index structures.
         * @note Expects each line in the file to contain 1–3 whitespace-separated symbols.
         */
		void loadGrammarFile();

	
		/**
         * @brief Constructs a Grammar object and loads the grammar from file.
         * @param grammarFilePath Path to a text file where each line is a grammar rule.
         */
		explicit Grammar(const std::string& grammarFilePath); 

		/**
         * @brief Get all epsilon (unary) production rules.
         * @return Vector of epsilon rules, each represented as a vector of label IDs.
         */
		inline const std::vector<std::vector<uint>>& getRule1() const
		{
			return grammar1_;
		}

		/**
         * @brief Get all unary production rules (A ::= B).
         * @return Vector of unary rules, each represented as a vector of label IDs.
         */
		inline const std::vector<std::vector<uint>>& getRule2() const
		{
			return grammar2_;
		}

		/**
         * @brief Get all binary production rules (A ::= B C).
         * @return Vector of binary rules, each represented as a vector of label IDs.
         */
		inline const std::vector<std::vector<uint>>& getRule3() const
		{
			return grammar3_;
		}

		/**
         * @brief Check if there is any epsilon production for a given symbol.
         * @param symbol Label ID to check.
         * @return True if an epsilon production exists for the symbol.
         */
		inline const bool checkRule1(uint symbol) const 
		{
			return grammar1index_.at(symbol);
		}

		/**
         * @brief Lookup all left-hand side labels A for rules A ::= B.
         * @param symbol Label ID B.
         * @return Vector of label IDs A such that A ::= B.
         */
		inline const std::vector<uint>& rule2Index(uint symbol) const 
		{
			return grammar2index_.at(symbol);
		}

		/**
         * @brief Lookup all left-hand side labels A for rules A ::= B C.
         * @param symbol1 Label ID B.
         * @param symbol2 Label ID C.
         * @return Vector of label IDs A such that A ::= B C.
         */
		inline const std::vector<uint>& rule3Index(uint symbol1, uint symbol2) const
		{
			return grammar3index_.at(symbol1 * labelSize_ + symbol2);
		}


		inline const std::vector<std::vector<uint>>& getRule2Index() const 
		{
			return grammar2index_;
		}


		inline const std::vector<std::vector<uint>>& getRule3Index() const
		{
			return grammar3index_;
		}

		/**
         * @brief Lookup pairs (A,C) for rules A ::= B C by B.
         * @param symbol Label ID B.
         * @return Vector of pairs (A,C) such that A ::= B C.
         */
		inline const std::vector<std::pair<uint,uint>>& rule3LeftIndex(uint symbol) const
		{
			return grammar3indexLeft_.at(symbol);
		}

		/**
         * @brief Lookup pairs (A,B) for rules A ::= B C by C.
         * @param symbol Label ID C.
         * @return Vector of pairs (A,B) such that A ::= B C.
         */
		inline const std::vector<std::pair<uint,uint>>& rule3RightIndex(uint symbol) const
		{
			return grammar3indexRight_.at(symbol);
		}

		inline const std::vector<std::vector<std::pair<uint, uint>>>& getRule3LeftIndex() const
		{
			return grammar3indexLeft_;
		}

		inline const std::vector<std::vector<std::pair<uint, uint>>>& getRule3RightIndex() const
		{
			return grammar3indexRight_;
		}

		/**
         * @brief Get the total number of unique labels in the grammar.
         * @return Number of unique labels.
         */
		inline uint getLabelSize() const 
		{
			return labelSize_;
		}

		/**
         * @brief Get the mapping from label strings to IDs.
         * @return Constant reference to the symbol-to-ID map.
         */
		inline const std::unordered_map<std::string,uint>& getSymbolToIDMap() const
		{
			return hashSym_;
		}

		/**
         * @brief Get the mapping from label IDs to strings.
         * @return Constant reference to the ID-to-symbol map.
         */
		inline const std::unordered_map<uint,std::string>& getIDToSymbolMap() const
		{
			return hashSymRev_;
		}
	};

} // namespace gracfl