/*
 * @BEGIN LICENSE
 *
 * Forte: an open-source plugin to Psi4 (https://github.com/psi4/psi4)
 * that implements a variety of quantum chemistry methods for strongly
 * correlated electrons.
 *
 * Copyright (c) 2012-2017 by its authors (see COPYING, COPYING.LESSER, AUTHORS).
 *
 * The copyrights for code used from other parties are included in
 * the corresponding files.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 *
 * @END LICENSE
 */

#ifndef _ex_aci_h_
#define _ex_aci_h_

#include <fstream>

#include <libmints/wavefunction.h>
#include <liboptions/liboptions.h>
#include <physconst.h>

#include "integrals.h"
#include "string_determinant.h"
#include "dynamic_bitset_determinant.h"

namespace psi{ namespace forte{

/**
 * @brief The Ex_ACI class
 * This class implements an adaptive CI algorithm and includes
 * several options for computing excited states (eventually)
 */
class EX_ACI : public Wavefunction
{
public:
    // ==> Class Constructor and Destructor <==

    /**
     * Constructor
     * @param ref_wfn The reference wavefunction object
     * @param options The main options object
     * @param ints A pointer to an allocated integral object
     */
    EX_ACI(boost::shared_ptr<Wavefunction> wfn, Options &options, ForteIntegrals* ints);

    /// Destructor
    ~EX_ACI();

    // ==> Class Interface <==

    /// Compute the energy
    double compute_energy();


private:

    // ==> Class data <==

    /// A reference to the options object
    Options& options_;
    /// The molecular integrals required by Explorer
    ForteIntegrals* ints_;
    /// The wave function symmetry
    int wavefunction_symmetry_;
    /// The symmetry of each orbital in Pitzer ordering
    std::vector<int> mo_symmetry_;
    ///The multiplicity of the reference
    int wavefunction_multiplicity_;
    /// The number of correlated molecular orbitals
    int ncmo_;
    /// The number of correlated electrons
    int ncel_;
    ///The number of frozen core orbitals
    int nfrzc_;
    /// The number of correlated molecular orbitals per irrep
    Dimension ncmopi_;
    /// The nuclear repulsion energy
    double nuclear_repulsion_energy_;
    /// The reference determinant
    StringDeterminant reference_determinant_;
    ///Current interation cycle
    int cycle_;
    /// The PT2 energy correction
    std::vector<double> multistate_pt2_energy_correction_;

    /// The threshold applied to the primary space
    double tau_p_;
    /// The threshold applied to the secondary space
    double tau_q_;
    /// The number of roots computed
    int nroot_;
    ///Use threshold from perturbation theory
    bool perturb_select_;
    ///The function of the q-space criteria per root
    std::string pq_function_;
    ///The type of q criteria
    bool q_rel_;
    ///The q reference
    std::string q_reference_;
    ///Algorithm for computing excited states
    std::string ex_alg_;
    ///The reference root
    int ref_root_;
    /// Enable aimed selection
    bool aimed_selection_;
    /// If true select by energy, if false use first-order coefficient
    bool energy_selection_;
    /// Smooth the Hamiltonian in the P space?
    bool do_smooth_;
    /// The threshold for smoothing elements of the Hamiltonian
    double smooth_threshold_;
    ///Number of roots to calculate for final excited state
    int post_root_;
    ///Rediagonalize Hamiltonian?
    bool post_diagonalize_;
    ///Print warning?
    bool print_warning_;
    ///Spin tolerance
    double spin_tol_;
    ///Compute 1-RDM?
    bool form_1_RDM_;
	///Enorce spin complete spaces?
	bool spin_complete_;

    /// A vector of determinants in the P space
    std::vector<DynamicBitsetDeterminant> P_space_;
    /// A vector of determinants in the P + Q space
    std::vector<DynamicBitsetDeterminant> PQ_space_;
    /// A map of determinants in the P space
    std::map<DynamicBitsetDeterminant,int> P_space_map_;

    ///Vector to store spin of each root
    std::vector<std::pair<double,double> > root_spin_vec_;
    ///1 particle density matrix
    SharedMatrix D1_;
    ///Form initial guess space with correct spin?
    bool do_guess_;
	///Spin-symmetrized evecs
	SharedMatrix PQ_spin_evecs_;


    // ==> Class functions <==

    /// All that happens before we compute the energy
    void startup();

    ///Get the reference occupation
    std::vector<int> get_occupation();

    /// Print information about this calculation
    void print_info();

    /// Print a wave function
    void print_wfn(std::vector<DynamicBitsetDeterminant> space, SharedMatrix evecs, int nroot);

    /// Diagonalize the Hamiltonian in a space of determinants
    void diagonalize_hamiltonian(const std::vector<DynamicBitsetDeterminant>& space, SharedVector &evals, SharedMatrix &evecs, int nroot);

    /// Diagonalize the Hamiltonian in a space of determinants
    void diagonalize_hamiltonian2(const std::vector<DynamicBitsetDeterminant>& space, SharedVector &evals, SharedMatrix &evecs, int nroot);

    /// Find all the relevant excitations out of the P space
    void find_q_space(int nroot, SharedVector evals, SharedMatrix evecs);

    /// Generate set of state-averaged q-criteria and determinants
    double average_q_values(int nroot, std::vector<std::pair<double,double> >C1, std::vector<std::pair<double,double> > E1);

    ///Select specific root to create q space
    double root_select(int nroot,std::vector<std::pair<double,double> > C1, std::vector<std::pair<double,double> > E2);

    /// Find all the relevant excitations out of the P space - single root version
    void find_q_space_single_root(int nroot, SharedVector evals, SharedMatrix evecs);

    /// Generate excited determinants
    void generate_excited_determinants_single_root(int nroot, int I, SharedMatrix evecs, DynamicBitsetDeterminant &det, std::map<DynamicBitsetDeterminant, double> &V_hash);

    /// Generate excited determinants
    void generate_excited_determinants(int nroot, int I, SharedMatrix evecs, DynamicBitsetDeterminant &det, std::map<DynamicBitsetDeterminant,std::vector<double>>& V_hash);

    /// Experimental
    void generate_pair_excited_determinants(int nroot,int I,SharedMatrix evecs,DynamicBitsetDeterminant& det,std::map<DynamicBitsetDeterminant,std::vector<double>>& V_hash);

    /// Prune the space of determinants
    void prune_q_space(std::vector<DynamicBitsetDeterminant>& large_space,std::vector<DynamicBitsetDeterminant>& pruned_space,
                                   std::map<DynamicBitsetDeterminant,int>& pruned_space_map,SharedMatrix evecs,int nroot);

    void smooth_hamiltonian(std::vector<DynamicBitsetDeterminant>& space,SharedVector evals,SharedMatrix evecs,int nroot);

    /// Check if the procedure has converged
    bool check_convergence(std::vector<std::vector<double>>& energy_history,SharedVector new_energies);

    ///Check if the procedure is stuck
    bool check_stuck(std::vector<std::vector<double>>& energy_history, SharedVector evals);

    /// Shrink the PQ space to include only max_det_ determinants
    void shrink_pq_space(std::vector<DynamicBitsetDeterminant>& total_space,std::vector<DynamicBitsetDeterminant>& pruned_space,
                         std::map<DynamicBitsetDeterminant,int>& pruned_space_map,SharedMatrix evecs, int nroot);
    ///Analyze the wavefunction
    void wfn_analyzer(std::vector<DynamicBitsetDeterminant> det_space, SharedMatrix evecs,int nroot);

    ///Take the direct product of two symmetry elements (int)
    int direct_sym_product(int sym1, int sym2);


    /// Returns a vector of orbital energy,sym label pairs
    std::vector<std::pair<double, std::pair<int, int> > > sym_labeled_orbitals(std::string type);

    ///Computes S^2 and S
    std::vector< std::pair<std::pair<double,double>, std::pair<size_t,double> > >compute_spin(std::vector<DynamicBitsetDeterminant> space, SharedMatrix evecs, int nroot);

    ///Compute 1 particle RDM
    void compute_1rdm(SharedMatrix A, SharedMatrix B,std::vector<DynamicBitsetDeterminant> det_space, SharedMatrix evecs,int nroot);

    ///One electron operator
    double OneOP(const DynamicBitsetDeterminant &J, DynamicBitsetDeterminant &Jnew, const bool sp, const size_t &p, const size_t &q);

    ///Check the sign
    double CheckSign(std::vector<int> I, const int &n);

    ///Form initial space of determinants with correct S^2 for each root
    void form_initial_space(std::vector<DynamicBitsetDeterminant> det_space, int nroot);

    void add_spin_pair(std::vector<DynamicBitsetDeterminant> det_space);

    ///Compute S^2 matrix and diagonalize it
    void spin_transform(std::vector<DynamicBitsetDeterminant> det_space, SharedMatrix cI, int nroot);

	///Check for spin complete determinants
	void check_spin_completeness(std::vector<DynamicBitsetDeterminant>& det_space);

//    int david2(double **A, int N, int M, double *eps, double **v,double cutoff, int print);
//    /// Perform a Davidson-Liu diagonalization
//    void davidson_liu(SharedMatrix H,SharedVector Eigenvalues,SharedMatrix Eigenvectors,int nroots);

//    /// Perform a Davidson-Liu diagonalization on a sparse matrix
//    bool davidson_liu_sparse(std::vector<std::vector<std::pair<int,double> > > H_sparse,SharedVector Eigenvalues,SharedMatrix Eigenvectors,int nroots);
};

}} // End Namespaces

#endif // _ex_aci_h_
