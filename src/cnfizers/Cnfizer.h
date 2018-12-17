/*********************************************************************
Author: Antti Hyvarinen <antti.hyvarinen@gmail.com>

OpenSMT2 -- Copyright (C) 2012 - 2014 Antti Hyvarinen
                         2008 - 2012 Roberto Bruttomesso

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************/

#ifndef CNFIZER_H
#define CNFIZER_H

#include "Global.h"
#include "Theory.h"
#include "Logic.h"

class SimpSMTSolver;
class CnfState;
class THandler;
struct SMTConfig;
class TermMapper;

// A small typechecked class for checkClause type return values to enable us to check whether the formula is unsatisfiable simultaneously
class ckval {
    char value;
    explicit ckval(int v) : value(v) { }
public:
    ckval()          : value(0) { }
    int toInt(void) const { return value; }
    bool operator == (ckval c) const { return value == c.value; }
    bool operator != (ckval c) const { return value == c.value; }

    friend ckval toCkval(int v);
};
inline ckval toCkval(int v) {return ckval(v); }

const ckval ck_True  = toCkval( 1);
const ckval ck_False = toCkval(-1);
const ckval ck_Unsat = toCkval( 0);



//
// Generic class for conversion into CNF
//
class Cnfizer
{
public:
    SimpSMTSolver&      solver;
protected:
    SMTConfig&          config;
    Theory&             theory;
    Logic&              logic;
    TermMapper&         tmap;

    THandler&           thandler;
    bool                s_empty;

public:

    Cnfizer( SMTConfig &    config_
           , Theory&        logic_
           , TermMapper&    tmap_
           , THandler&      thandler_
           , SimpSMTSolver& solver_
           );


    virtual ~Cnfizer( ) { }

    lbool cnfizeAndGiveToSolver (PTRef, FrameId frame_id); // Main routine

    lbool  getTermValue(PTRef) const;

    void   initialize      ();
    lbool  solve           (vec<FrameId>& en_frames);

    bool  isNPAtom         (PTRef r, PTRef& p)    const; // Check if r is a (negated) atom.  Return true if the corresponding atom is negated.  The purified reference is placed in the second argument.
    bool  solverEmpty      ()                     const { return s_empty; }

    void  getSolverState   (CnfState& cs);
    void  getVarMapping    (CnfState&);
protected:

    virtual bool cnfize                 ( PTRef ) = 0; // Actual cnfization. To be implemented in derived classes
    bool     deMorganize                ( PTRef );                                    // Apply deMorgan rules whenever feasible


public:
    bool     checkClause                ( PTRef ); // Check if a formula is a clause
    bool     checkCnf                   ( PTRef );                            // Check if formula is in CNF
    bool     checkDeMorgan              ( PTRef );                            // Check if formula can be deMorganized
    void     retrieveTopLevelFormulae   ( PTRef, vec<PTRef> & );         // Retrieves the list of top-level formulae
protected:

    bool     giveToSolver               ( PTRef );                              // Gives formula to the SAT solver


    bool addClause(const vec<Lit> &);

    void  retrieveClause             ( PTRef, vec<PTRef> & );         // Retrieve a clause from a formula
    void  retrieveConjuncts          ( PTRef, vec<PTRef> & );         // Retrieve the list of conjuncts

private:

    bool    checkConj            (PTRef); // Check if a formula is a conjunction
    bool    checkPureConj        (PTRef, Map<PTRef,bool,PTRefHash>& check_cache); // Check if a formula is purely a conjuntion

protected:

    // PROOF version
    int currentPartition = -1;
    // end of PROOF version

    PTRef frame_term;
    vec<PTRef> frame_terms;
    void setFrameTerm(FrameId frame_id);
};

#endif
